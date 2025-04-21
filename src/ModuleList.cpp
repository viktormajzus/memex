#include <memex/ModuleList.hpp>

#include "pch.hpp"

ModuleList::ModuleList(const ProcessHandle& process)
  : m_process{ &process }
{

}

std::expected<ModuleList, memex::Error> ModuleList::Create(const ProcessHandle& process)
{
  if (!process.isAttached())
    return std::unexpected(memex::Error::InvalidHandle);

  ModuleList modules{ process };
  modules.m_modules.reserve(128); // Reserve a upper bound

  auto refreshStatus{ modules.refresh() };
  if (!refreshStatus)
    return std::unexpected(refreshStatus.error());

  return modules;
}

std::expected<void, memex::Error> ModuleList::refresh()
{
  m_modules.clear();

  HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_process->processId()) };
  if (snapshot == INVALID_HANDLE_VALUE)
    return std::unexpected(memex::Error::SnapshotFailed);

  MODULEENTRY32 moduleEntry{ .dwSize = sizeof(moduleEntry) };
  if (!Module32First(snapshot, &moduleEntry))
  {
    CloseHandle(snapshot);
    return std::unexpected(memex::Error::ModFirstFailed);
  }

  do
  {
    auto module{ Module{
      *m_process,
      RemotePtr<void>(reinterpret_cast<std::uintptr_t>(moduleEntry.modBaseAddr)),
      static_cast<std::size_t>(moduleEntry.modBaseSize),
      typed::tstring_view{
        moduleEntry.szModule,
        typed::tstrlen(moduleEntry.szModule, std::size(moduleEntry.szModule))
        }
      }
    };

    m_modules.push_back(module);
  } while (Module32Next(snapshot, &moduleEntry));

  CloseHandle(snapshot);
  return{};
}

std::optional<Module> ModuleList::find(typed::tstring_view name) const
{
  auto it{ std::find_if(m_modules.begin(), m_modules.end(),
               [name](const Module& mod) -> bool
               {
                 return mod.name() == name;
               }) };

  if (it != m_modules.end())
    return *it;

  return std::nullopt;
}

std::optional<Module> ModuleList::find(RemotePtr<void> address) const
{
  auto it{ std::find_if(m_modules.begin(), m_modules.end(),
               [address](const Module& mod) -> bool
               {
                 return mod.base() == address;
               }) };

  if (it != m_modules.end())
    return *it;

  return std::nullopt;
}

const std::vector<Module>& ModuleList::all() const
{
  return m_modules;
}

std::size_t ModuleList::size() const
{
  return std::size(m_modules);
}

void ModuleList::clear()
{
  m_modules.clear();
}

bool ModuleList::empty() const
{
  return m_modules.empty();
}

bool ModuleList::contains(typed::tstring_view name) const
{
  return find(name).has_value();
}

bool ModuleList::contains(RemotePtr<void> address) const
{
  return find(address).has_value();
}

std::optional<Module> ModuleList::findWithinRange(RemotePtr<void> address) const
{
  std::uintptr_t addr{ address.address() };
  auto it{ std::find_if(m_modules.begin(), m_modules.end(),
                       [addr](const Module& mod)
                        {
                         auto base{mod.base().address()};
                         return addr >= base && addr < base + mod.size();
                        }) };

  if (it != m_modules.end())
    return *it;

  return std::nullopt;
}
