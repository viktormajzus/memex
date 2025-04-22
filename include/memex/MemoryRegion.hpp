#pragma once
#include <memex/RemotePtr.hpp>

#include <cstdint>
#include <Windows.h>

struct MemoryRegion
{
  RemotePtr<void> m_baseAddress;
  std::size_t m_size{};
  DWORD m_protection;
  DWORD m_state{};
  DWORD m_type;

  bool contains(std::uintptr_t address) const;

  [[nodiscard]] bool isReadable() const;
  [[nodiscard]] bool isWritable() const;
  [[nodiscard]] bool isExecutable() const;
  [[nodiscard]] bool isCommitted() const;
};
