#pragma once
#include <cstdint>
#include <span>
#include <memex/ProcessHandle.hpp>
#include <memex/RemotePtr.hpp>

/**
 * @brief This class provides an interface for reading and writing memory in a remote process.
 * @note Copy constructor and assignment operator are deleted to prevent copying.
 * @note Move constructor and assignment operator are defaulted to allow moving.
 */
class MemoryIO
{
private:
  const ProcessHandle *m_process{};
  std::uint8_t m_ptrSize{};

public:
  /**
   * @brief Construct a new MemoryIO object
   *
   * @param process The process handle to the target process.
   */
  explicit MemoryIO(const ProcessHandle &process);

  MemoryIO(const MemoryIO &) = delete;
  MemoryIO &operator=(const MemoryIO &) = delete;
  MemoryIO(MemoryIO &&) noexcept = default;
  MemoryIO &operator=(MemoryIO &&) noexcept = default;

  [[nodiscard]]
  /**
   * @brief Getter for the pointer size.
   * @note The pointer size is determined by the architecture of the target process.
   * @return std::uint8_t The size of the pointer in bytes.
   */
  std::uint8_t pointerSize() const
  {
    return m_ptrSize;
  };

  [[nodiscard]]
  /**
   * @brief This function determines if the process is attached.
   *
   * @return true if the process is attached
   * @return false if the process is not attached
   */
  bool isAttached() const
  {
    return m_process && m_process->isAttached();
  };

  template <typename T>
  [[nodiscard]]
  /**
   * @brief Wrapper for ReadProcessMemory.
   *
   * @param address The address to read from.
   * @tparam T The type of the value to read.
   * @return T The value read from the address.
   */
  T Read(RemotePtr<T> address)
  {
    T buffer{};
    ReadProcessMemory(m_process->native(),
                      reinterpret_cast<void *>(address.address()),
                      &buffer, sizeof(T), 0);
    return buffer;
  }

  template <typename T>
  /**
   * @brief Wrapper for ReadProcessMemory. Similar to memcpy.
   *
   * @param dst Pointer to the destination buffer.
   * @param src The address to read from.
   * @param size The size of the buffer to read.
   * @return std::size_t The number of bytes read.
   * @note This function is not type-safe. It is the caller's responsibility to ensure that the size of the buffer is correct.
   */
  std::size_t Read(void *dst, RemotePtr<T> src, std::size_t size)
  {
    SIZE_T bytesRead{};
    ReadProcessMemory(m_process->native(),
                      reinterpret_cast<void *>(src.address()),
                      dst, static_cast<SIZE_T>(size), &bytesRead);
    return static_cast<std::size_t>(bytesRead);
  }

  template <typename T>
  /**
   * @brief Wrapper for WriteProcessMemory.
   *
   * @param address The address to write to.
   * @param value The value to write.
   * @tparam T The type of the value to write.
   * @note This function is not type-safe. It is the caller's responsibility to ensure that the size of the value is correct.
   */
  void Write(RemotePtr<T> address, const T &value)
  {
    WriteProcessMemory(m_process->native(),
                       reinterpret_cast<void *>(address.address()), &value,
                       sizeof(T), 0);
  }

  template <typename T>
  /**
   * @brief Wrapper for WriteProcessMemory. Writes a buffer to the target process.
   *
   * @param dst The address to write to.
   * @param src Pointer to the source buffer.
   * @param size The size of the buffer to write.
   * @return std::size_t The number of bytes written.
   * @note This function is not type-safe. It is the caller's responsibility to ensure that the size of the buffer is correct.
   */
  std::size_t Write(RemotePtr<T> dst, const void *src, std::size_t size)
  {
    SIZE_T bytesWritten{};
    WriteProcessMemory(m_process->native(),
                       reinterpret_cast<void *>(dst.address()),
                       src, static_cast<SIZE_T>(size), &bytesWritten);
    return static_cast<std::size_t>(bytesWritten);
  }

  template <typename T>
  [[nodiscard]]
  /**
   * @brief Resolves a pointer chain in the target process.
   *
   * @param base The base address of the pointer chain.
   * @param offsets A span of offsets to resolve the pointer chain.
   * @return RemotePtr<T> The resolved pointer.
   * @note This function is not type-safe. It is the caller's responsibility to ensure that the offsets are correct.
   * @note This function assumes that the base address is a valid pointer in the target process.
   */
  RemotePtr<T> ResolvePointerChain(RemotePtr<void> base, std::span<const std::ptrdiff_t> offsets)
  {
    if (offsets.empty())
      return RemotePtr<T>(base.address());

    std::uintptr_t current = Read<std::uintptr_t>(RemotePtr<std::uintptr_t>(base.address()));

    for (std::size_t i{0}; i < std::size(offsets) - 1; ++i)
    {
      current += offsets[i];
      current = Read<std::uintptr_t>(RemotePtr<std::uintptr_t>(current));
    }
    current += offsets.back();

    return RemotePtr<T>(current);
  }

  /**
   * @brief Fills a buffer in the target process with a specified byte (opcode).
   *
   * @param dst The address to fill.
   * @param value The byte value to fill the buffer with.
   * @param size The size of the buffer to fill.
   */
  void Fill(RemotePtr<void> dst, std::uint8_t value, std::size_t size);

  template <typename T>
  [[nodiscard]]
  /**
   * @brief Overload of ResolvePointerChain for std::initializer_list.
   *
   * @param base The base address of the pointer chain.
   * @param offsets A list of offsets to resolve the pointer chain.
   * @return RemotePtr<T> The resolved pointer.
   */
  RemotePtr<T> ResolvePointerChain(RemotePtr<void> base, std::initializer_list<std::ptrdiff_t> offsets)
  {
    if (offsets.size() == 0)
      return RemotePtr<T>(base.address());

    std::uintptr_t current = Read<std::uintptr_t>(RemotePtr<std::uintptr_t>(base.address()));

    for (auto it = offsets.begin(); it != offsets.end() - 1; ++it)
    {
      current += *it;
      current = Read<std::uintptr_t>(RemotePtr<std::uintptr_t>(current));
    }

    current += *(offsets.end() - 1);
    return RemotePtr<T>(current);
  }
};