#include <memex/MemoryRegion.hpp>

#include "pch.hpp"

bool MemoryRegion::contains(std::uintptr_t address) const
{
  auto base = m_baseAddress.address();
  return address >= base && address < base + m_size;
}

bool MemoryRegion::isReadable() const
{
  return m_protection & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE);
}

bool MemoryRegion::isWritable() const
{
  return m_protection & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE);
}

bool MemoryRegion::isExecutable() const
{
  return m_protection & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE);
}

bool MemoryRegion::isCommitted() const
{
  return m_state == MEM_COMMIT;
}
