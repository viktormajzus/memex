#pragma once
#include <memex/ProcessHandle.hpp>
#include <memex/error.hpp>
#include <memex/RemotePtr.hpp>
#include <memex/typed.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <expected>

class Module
{
private:
  const ProcessHandle* m_process{};
  RemotePtr<void> m_baseAddress{};
  std::size_t m_size{};
  typed::tstring m_name{};

  Module(const ProcessHandle& process, typed::tstring_view moduleName);

public:
  static std::expected<Module, memex::Error> Create(const ProcessHandle& process, typed::tstring_view moduleName);

  [[nodiscard]]
  RemotePtr<void> base() const;
  [[nodiscard]]
  std::size_t size() const;
  [[nodiscard]]
  typed::tstring_view name() const;

private:
  std::expected<RemotePtr<void>, memex::Error> GetModuleBaseAddress();
};