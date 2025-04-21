#include <memex/utility.hpp>

#include "pch.hpp"

std::expected<std::uint32_t, memex::Error> utility::FindPidByName(typed::tstring_view processName)
{
  HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
  if (snapshot == INVALID_HANDLE_VALUE)
    return std::unexpected(memex::Error::InvalidHandle);

  PROCESSENTRY32 processEntry{ .dwSize = sizeof(processEntry) };
  
  if (!Process32First(snapshot, &processEntry))
  {
    CloseHandle(snapshot);
    return std::unexpected(memex::Error::ProcFirstFailed);
  }

  do
  {
    if (processName.compare(processEntry.szExeFile))
      continue;

    CloseHandle(snapshot);
    return processEntry.th32ProcessID;
  } while (Process32Next(snapshot, &processEntry));

  CloseHandle(snapshot);
  return std::unexpected(memex::Error::ProcessNotFound);
}