#pragma once
#include <cstdint>
#include <string_view>
#include <expected>

#include <memex/error.hpp>
#include <memex/typed.hpp>

/**
 * @brief This file contains utility functions for the memex library.
 *
 */
namespace utility
{
  /**
   * @brief Find a process ID using its name.
   *
   * @param processName The name of the process to search for.
   * @note either a wide string or a narrow string can be used.
   * @return std::expected<std::uint32_t, memex::Error> The process ID if found, or an error if not.
   */
  std::expected<std::uint32_t, memex::Error> FindPidByName(typed::tstring_view processName);
}