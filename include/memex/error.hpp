#pragma once
#include <string_view>

namespace memex
{
  /**
   * @brief An enumeration of error codes that can be returned by the library.
   *
   */
  enum class Error
  {
    AlreadyAttached,
    InvalidPID,
    AccessDenied,
    OpenFailed,
    TerminateFailed,
    InvalidHandle,
    ProcessNotFound,
    ProcFirstFailed,
    SnapshotFailed,
    ModFirstFailed,
    ModuleNotFound
  };

  /**
   * @brief Converts an error code to its string representation.
   *
   * @param error The error code to convert.
   * @return constexpr std::string_view The string representation of the error code.
   */
  constexpr std::string_view parseError(Error error)
  {
    switch (error)
    {
    case Error::AlreadyAttached:
      return "Already attached";
    case Error::InvalidPID:
      return "Invalid Process Id";
    case Error::AccessDenied:
      return "Access denied";
    case Error::OpenFailed:
      return "Failed to open process";
    case Error::TerminateFailed:
      return "Failed to terminate process";
    case Error::InvalidHandle:
      return "Invalid handle";
    case Error::ProcessNotFound:
      return "Process not found";
    case Error::ProcFirstFailed:
      return "Process32First failed";
    case Error::SnapshotFailed:
      return "Failed to create Toolhelp snapshot";
    case Error::ModFirstFailed:
      return "Module32First failed";
    case Error::ModuleNotFound:
      return "Module not found";
    default:
      return "Unknown error";
    }
  }
}