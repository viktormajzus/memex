#pragma once
#include <Windows.h>
#include <cstdint>
#include <expected>
#include <string_view>

#include <memex/access_mask.hpp>
#include <memex/error.hpp>
#include <memex/typed.hpp>

/**
 * @brief Class to manage a process handle.
 * @note This class is used to manage a process handle. It provides methods to attach to a process, check if the handle is valid, and terminate the process.
 * @note Copy constructors are deleted to prevent copying of the handle.
 * @note Move constructors are provided to allow moving of the handle.
 */
class ProcessHandle
{
private:
  HANDLE m_handle{nullptr};
  std::uint32_t m_processId{};
  memex::AccessMask m_accessMask{};

public:
  /**
   * @brief Construct a new Process Handle object.
   *
   */
  ProcessHandle() = default;

  /**
   * @brief Destroy the Process Handle object.
   *
   */
  ~ProcessHandle();

  // Copy and move constructors
  ProcessHandle(const ProcessHandle &) = delete;
  ProcessHandle &operator=(const ProcessHandle &) = delete;
  ProcessHandle(ProcessHandle &&) noexcept;
  ProcessHandle &operator=(ProcessHandle &&) noexcept;

  /**
   * @brief This function is responsible for opening a process handle with the specified access rights.
   *
   * @param processId Process ID of the target process.
   * @param desiredAccess Access rights to be granted to the process handle.
   * @note The default access rights are set to full access.
   * @return std::expected<void, memex::Error> Returns an expected object containing either void on success or an error on failure.
   */
  std::expected<void, memex::Error> attach(std::uint32_t processId,
                                           memex::AccessMask desiredAccess = memex::AccessMask::Full);

  /**
   * @brief This function checks if the process handle is open
   *
   * @return true if the handle is open
   * @return false if the handle is not open
   */
  bool isAttached() const;

  /**
   * @brief This function returns a raw handle to the process.
   *
   * @return const HANDLE& Returns a reference to the raw handle.
   */
  const HANDLE &native() const;

  /**
   * @brief Getter for the process ID.
   *
   * @return std::uint32_t Returns the process ID.
   */
  std::uint32_t processId() const;

  /**
   * @brief This function attaches to a process by its name.
   *
   * @param processName Name of the process to attach to.
   * @note The process name should be in the format "process.exe".
   * @note Grants PROCESS_FULL_ACCESS
   * @return std::expected<void, memex::Error> Returns an expected object containing either void on success or an error on failure.
   */
  std::expected<void, memex::Error> attach(typed::tstring_view processName);

  /**
   * @brief Terminates the process with the specified exit code.
   *
   * @param exitCode Exit code to be used for termination.
   * @return std::expected<void, memex::Error> Returns an expected object containing either void on success or an error on failure.
   */
  std::expected<void, memex::Error> terminate(unsigned int exitCode);
};