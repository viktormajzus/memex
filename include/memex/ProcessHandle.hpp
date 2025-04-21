#pragma once
#include <Windows.h>
#include <cstdint>
#include <expected>
#include <string_view>

#include <memex/access_mask.hpp>
#include <memex/error.hpp>

class ProcessHandle
{
private:
  HANDLE m_handle{ nullptr };
  std::uint32_t m_processId{};
  memex::AccessMask m_accessMask{};

public:
  ProcessHandle() = default;

  ~ProcessHandle();

  ProcessHandle(const ProcessHandle&) = delete;
  ProcessHandle& operator=(const ProcessHandle&) = delete;
  ProcessHandle(ProcessHandle&&) noexcept;
  ProcessHandle& operator=(ProcessHandle&&) noexcept;

  std::expected<void, memex::Error> attach(std::uint32_t processId,
                                           memex::AccessMask desiredAccess = memex::AccessMask::Full);

  bool isAttached() const;

#ifdef UNICODE
  using tstring_view = std::wstring_view;
#else
  using tstring_view = std::string_view;
#endif

  std::expected<void, memex::Error> attach(tstring_view processName);

  std::expected<void, memex::Error> terminate(unsigned int exitCode);
};