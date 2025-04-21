#include <memex/ProcessHandle.hpp>
#include <memex/utility.hpp>

#include "pch.hpp"

ProcessHandle::~ProcessHandle()
{
  if (m_handle)
    CloseHandle(m_handle);
}

ProcessHandle::ProcessHandle(ProcessHandle&& other) noexcept
  : m_handle{ other.m_handle }
  , m_processId{ other.m_processId }
  , m_accessMask{ other.m_accessMask }
{
  other.m_handle = nullptr;
}

ProcessHandle& ProcessHandle::operator=(ProcessHandle&& other) noexcept
{
  if (&other == this)
    return *this;

  if (m_handle)
    CloseHandle(m_handle);

  m_handle = other.m_handle;
  other.m_handle = nullptr;
  m_processId = other.m_processId;
  m_accessMask = other.m_accessMask;

  return *this;
}

std::expected<void, memex::Error> ProcessHandle::attach(std::uint32_t processId, memex::AccessMask desiredAccess)
{
  if (m_handle)
    return std::unexpected(memex::Error::AlreadyAttached);

  HANDLE h = OpenProcess(static_cast<DWORD>(desiredAccess),
                         false,
                         processId);

  if (!h)
  {
    if (GetLastError() == ERROR_ACCESS_DENIED)
      return std::unexpected(memex::Error::AccessDenied);
    return std::unexpected(memex::Error::OpenFailed);
  }

  m_handle = h;
  m_processId = processId;
  m_accessMask = desiredAccess;

  return{};
}

bool ProcessHandle::isAttached() const
{
  return m_handle != nullptr;
}

const HANDLE& ProcessHandle::native() const
{
  return m_handle;
}

std::uint32_t ProcessHandle::processId() const
{
  return m_processId;
}

std::expected<void, memex::Error> ProcessHandle::attach(typed::tstring_view processName)
{
  auto processId{ utility::FindPidByName(processName) };
  if (!processId)
    return std::unexpected(processId.error());

  auto result{ attach(processId.value()) };
  if (!result)
    return result;

  return{};
}

std::expected<void, memex::Error> ProcessHandle::terminate(unsigned int exitCode)
{
  if (!m_handle)
    return std::unexpected(memex::Error::InvalidHandle);

  if (!TerminateProcess(m_handle, exitCode))
    return std::unexpected(memex::Error::TerminateFailed);

  return{};
}
