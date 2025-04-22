#pragma once
#include <memex/ProcessHandle.hpp>
#include <memex/MemoryRegion.hpp>
#include <memex/error.hpp>
#include <memex/RemotePtr.hpp>

#include <vector>
#include <expected>
#include <optional>

class MemoryMap
{
private:
  const ProcessHandle* m_process{};
  std::vector<MemoryRegion> m_regions;

public:
  explicit MemoryMap(const ProcessHandle& process);
  std::expected<void, memex::Error> refresh();
  const std::vector<MemoryRegion>& regions() const;

  std::optional<MemoryRegion> find(RemotePtr<void> address) const;
  std::vector<MemoryRegion> withProtection(DWORD protection) const;
  bool isReadable(RemotePtr<void> address) const;
};
