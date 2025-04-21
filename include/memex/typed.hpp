#pragma once
#include <string>
#include <string_view>

/**
 * @brief This file contains type definitions for string types that are compatible with both ANSI and Unicode builds.
 *
 */
namespace typed
{
#ifdef UNICODE
  using tstring = std::wstring;
  using tstring_view = std::wstring_view;
#else
  using tstring = std::string;
  using tstring_view = std::string_view;
#endif
}