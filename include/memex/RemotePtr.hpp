#pragma once
#include <cstdint>
#include <cstddef>

template<typename T>
class RemotePtr
{
private:
  std::uintptr_t m_address{ nullptr };

public:
  RemotePtr() = default;
  explicit RemotePtr(std::uintptr_t address) : m_address{ address } {}

  template<typename U>
  RemotePtr<U> as()
  {
    return RemotePtr<U>(m_address);
  }

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

  RemotePtr<T>& operator+=(std::ptrdiff_t offset)
  {
    m_address += offset;
    return *this;
  }
  RemotePtr<T>& operator-=(std::ptrdiff_t offset)
  {
    m_address -= offset;
    return *this;
  }

  explicit operator bool() const { return m_address != 0; };
  bool operator==(const RemotePtr<T>& other) const { return m_address == other.m_address; }
  bool operator!=(const RemotePtr<T>& other) const { return m_address != other.m_address; }
  bool operator<(const RemotePtr<T>& other) const { return m_address < other.m_address; }

};