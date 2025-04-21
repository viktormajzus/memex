#include <memex/MemoryIO.hpp>

#include "pch.hpp"

MemoryIO::MemoryIO(const ProcessHandle& process)
  : m_process{ &process }
{
  assert(process.isAttached() && "ProcessHandle must be attached");

  BOOL isWow64;
  IsWow64Process(process.native(), &isWow64);
  m_ptrSize = isWow64 ? 4 : 8;
}

void MemoryIO::Fill(RemotePtr<void> dst, std::uint8_t value, std::size_t size)
{
  std::vector<std::uint8_t> buffer(size, value);
  Write(dst, buffer.data(), size);
}
