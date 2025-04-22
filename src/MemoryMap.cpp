#include <memex/MemoryMap.hpp>

#include "pch.hpp"

MemoryMap::MemoryMap(const ProcessHandle& process)
  : m_process{ &process }
{
  m_regions.reserve(2048); // Conservative reserve
  refresh();
}

std::expected<void, memex::Error> MemoryMap::refresh()
{
  if (!m_process || !m_process->isAttached())
    return std::unexpected(memex::Error::InvalidHandle);

  m_regions.clear();

  MEMORY_BASIC_INFORMATION mbi{};
  std::uintptr_t address{};
  const std::uintptr_t maxAddress{ ~0ull };

  while (VirtualQueryEx(m_process->native(),
                        reinterpret_cast<void*>(address),
                        &mbi, sizeof(mbi)) == sizeof(mbi))
  {
    if (mbi.State == MEM_COMMIT)
    {
      m_regions.push_back(MemoryRegion{
        RemotePtr<void>(reinterpret_cast<std::uintptr_t>(mbi.BaseAddress)),
        mbi.RegionSize,
        mbi.State,
        mbi.Protect,
        mbi.Type
                          });
    }

    address += mbi.RegionSize;
    if (address >= maxAddress)
      break;
  }

  m_regions.shrink_to_fit();

  return{};
}

const std::vector<MemoryRegion>& MemoryMap::regions() const
{
  return m_regions;
}

std::optional<MemoryRegion> MemoryMap::find(RemotePtr<void> address) const
{
  std::uintptr_t target{ address.address() };
  std::size_t left{};
  std::size_t right{ m_regions.size() };

  while (left < right)
  {
    std::size_t mid{ left + (right - left) / 2 };
    const MemoryRegion& region{ m_regions[mid] };

    auto base{ region.m_baseAddress.address() };

    if (target < base)
      right = mid;
    else if (target >= base + region.m_size)
      left = mid + 1;
    else
      return region;
  }

  return std::nullopt;
}

std::vector<MemoryRegion> MemoryMap::withProtection(DWORD protection) const
{
  std::vector<MemoryRegion> regions;
  regions.reserve(m_regions.size());

  for (const auto& region : m_regions)
  {
    if (region.m_protection != protection)
      continue;

    regions.push_back(region);
  }

  regions.shrink_to_fit();
  return regions;
}

bool MemoryMap::isReadable(RemotePtr<void> address) const
{
  auto regionOpt{ find(address) };
  if (!regionOpt.has_value())
    return false;

  return regionOpt->isReadable();
}
