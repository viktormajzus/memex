#pragma once
#include <cstdint>
#include <string_view>
#include <expected>

#include <memex/error.hpp>

namespace utility
{
#ifdef UNICODE
  using tstring_view = std::wstring_view;
#else
  using tstring_view = std::string_view;
#endif

  std::expected<std::uint32_t, memex::Error> FindPidByName(tstring_view processName);
}