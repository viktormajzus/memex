#pragma once
#include <cstdint>
#include <cstddef>

template <typename T>
/** RemotePtr class
 * @brief A class that represents a pointer to a remote memory address.
 *
 */
class RemotePtr
{
private:
  std::uintptr_t m_address{};

public:
  /**
   * @brief Construct a new Remote Ptr object
   *
   */
  RemotePtr() = default;
  /**
   * @brief Construct a new Remote Ptr object, using a raw address.
   *
   * @param address The address to point to.
   * @note This constructor is explicit to prevent implicit conversions from raw addresses to RemotePtr.
   */
  explicit RemotePtr(std::uintptr_t address) : m_address{address} {}

  template <typename U>
  /**
   * @brief Convert this RemotePtr to a RemotePtr of another type.
   *
   * @return RemotePtr<U> A RemotePtr of the new type, pointing to the same address.
   * @note This is a template function to allow conversion to any type.
   */
  RemotePtr<U> as()
  {
    return RemotePtr<U>(m_address);
  }

  /**
   * @brief Getter for the address.
   *
   * @return std::uintptr_t The address of the pointer.
   */
  std::uintptr_t address() const { return m_address; };

  // Operator overloads
  RemotePtr<T> operator+(std::ptrdiff_t offset) const
  {
    return RemotePtr<T>(m_address + offset);
  }
  RemotePtr<T> operator-(std::ptrdiff_t offset) const
  {
    return RemotePtr<T>(m_address - offset);
  }

  RemotePtr<T> &operator+=(std::ptrdiff_t offset)
  {
    m_address += offset;
    return *this;
  }
  RemotePtr<T> &operator-=(std::ptrdiff_t offset)
  {
    m_address -= offset;
    return *this;
  }

  explicit operator bool() const { return m_address != 0; };
  bool operator==(const RemotePtr<T> &other) const { return m_address == other.m_address; }
  bool operator!=(const RemotePtr<T> &other) const { return m_address != other.m_address; }
  bool operator<(const RemotePtr<T> &other) const { return m_address < other.m_address; }
};