#pragma once
#include <cstdint>
#include <span>
#include <memex/ProcessHandle.hpp>
#include <memex/RemotePtr.hpp>

class MemoryIO
{
private:
  const ProcessHandle* m_process{};
  std::uint8_t m_ptrSize{};

public:
  explicit MemoryIO(const ProcessHandle& process);

  MemoryIO(const MemoryIO&) = delete;
  MemoryIO& operator=(const MemoryIO&) = delete;
  MemoryIO(MemoryIO&&) noexcept = default;
  MemoryIO& operator=(MemoryIO&&) noexcept = default;

  [[nodiscard]]
  std::uint8_t pointerSize() const { return m_ptrSize; };

  [[nodiscard]]
  bool isAttached() const { return m_process && m_process->isAttached(); };

  template<typename T>
  [[nodiscard]]
  T Read(RemotePtr<T> address)
  {
    T buffer{};
    ReadProcessMemory(m_process->native(),
                      reinterpret_cast<void*>(address.address()),
                      &buffer, sizeof(T), 0);
    return buffer;
  }
  
  template<typename T>
  std::size_t Read(void* dst, RemotePtr<T> src, std::size_t size)
  {
    SIZE_T bytesRead{};
    ReadProcessMemory(m_process->native(),
                      reinterpret_cast<void*>(src.address()),
                      dst, static_cast<SIZE_T>(size), &bytesRead);
    return static_cast<std::size_t>(bytesRead);
  }

  template<typename T>
  void Write(RemotePtr<T> address, const T& value)
  {
    WriteProcessMemory(m_process->native(),
                       reinterpret_cast<void*>(address.address()), &value,
                       sizeof(T), 0);
  }

  template<typename T>
  std::size_t Write(RemotePtr<T> dst, const void* src, std::size_t size)
  {
    SIZE_T bytesWritten{};
    WriteProcessMemory(m_process->native(),
                       reinterpret_cast<void*>(dst.address()),
                       src, static_cast<SIZE_T>(size), &bytesWritten);
    return static_cast<std::size_t>(bytesWritten);
  }

  template<typename T>
  [[nodiscard]]
  RemotePtr<T> ResolvePointerChain(RemotePtr<void> base, std::span<const std::ptrdiff_t> offsets)
  {
    if (offsets.empty())
      return RemotePtr<T>(base.address());

    std::uintptr_t current = Read(base);

    for (std::size_t i{ 0 }; i < std::size(offsets) - 1; ++i)
    {
      current += offsets[i];
      current = Read(RemotePtr<void>(current));
    }
    current += offsets.back();

    return RemotePtr<T>(current);
  }

  void Fill(RemotePtr<void> dst, std::uint8_t value, std::size_t size);

  template<typename T>
  [[nodiscard]]
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