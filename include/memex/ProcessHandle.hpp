#pragma once
#include <Windows.h>
#include <cstdint>
#include <expected>
#include <string_view>

#include <memex/access_mask.hpp>
#include <memex/error.hpp>
#include <memex/typed.hpp>

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

  const HANDLE& native() const;

  std::uint32_t processId() const;

  std::expected<void, memex::Error> attach(typed::tstring_view processName);

  std::expected<void, memex::Error> terminate(unsigned int exitCode);
};