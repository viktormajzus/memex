#pragma once
#include <cstring>
#include <cwchar>

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
  using tchar = wchar_t;

  inline std::size_t tstrlen(const tchar* str, std::size_t max = 256)
  {
    return wcsnlen(str, max);
  }
#else
  using tstring = std::string;
  using tstring_view = std::string_view;
  using tchar = char;

  inline std::size_t tstrlen(const tchar* str, std::size_t max = 256)
  {
    return strnlen(str, max);
  }
#endif
}