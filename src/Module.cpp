#include <memex/Module.hpp>
#include <memex/error.hpp>

#include "pch.hpp"

Module::Module(const ProcessHandle& process, typed::tstring_view moduleName)
  : m_process{ &process }
  , m_name{ moduleName }
{

}

Module::Module(const ProcessHandle& process,
               const RemotePtr<void>& baseAddress,
               std::size_t size, typed::tstring_view name)
  : m_process{ &process }
  , m_baseAddress{ baseAddress }
  , m_size{ size }
  , m_name{ name }
{

}

std::expected<Module, memex::Error> Module::Create(const ProcessHandle& process, typed::tstring_view moduleName)
{
  assert(process.isAttached() && "Process must be attached");

  Module mod{ process, moduleName };

  auto baseAddress{ mod.GetModuleBaseAddress() };
  if (!baseAddress)
    return std::unexpected(baseAddress.error());

  mod.m_baseAddress = baseAddress.value();
  return mod;
}

RemotePtr<void> Module::base() const
{
  return m_baseAddress;
}

std::size_t Module::size() const
{
  return m_size;
}

typed::tstring_view Module::name() const
{
  return m_name;
}

std::expected<RemotePtr<void>, memex::Error> Module::GetModuleBaseAddress()
{
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
    if (m_name.compare(moduleEntry.szModule))
      continue;

    CloseHandle(snapshot);
    m_size = moduleEntry.modBaseSize;
    return RemotePtr<void>(reinterpret_cast<std::uintptr_t>(moduleEntry.modBaseAddr));
  } while (Module32Next(snapshot, &moduleEntry));

  CloseHandle(snapshot);
  return std::unexpected(memex::Error::ModuleNotFound);
}
