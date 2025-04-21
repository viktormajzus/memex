#pragma once
#include <memex/ProcessHandle.hpp>
#include <memex/error.hpp>
#include <memex/RemotePtr.hpp>
#include <memex/typed.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <expected>

/**
 * @brief Represents a module in a process.
 * @note Create an instance using the static Create method.
 */
class Module
{
private:
  const ProcessHandle *m_process{};
  RemotePtr<void> m_baseAddress{};
  std::size_t m_size{};
  typed::tstring m_name{};

  // Private constructor to enforce the use of Create method
  Module(const ProcessHandle &process, typed::tstring_view moduleName);

public:
  explicit Module(const ProcessHandle& process,
                  const RemotePtr<void>& baseAddress,
                  std::size_t size,
                  typed::tstring_view name);
  /**
   * @brief Creates a Module instance.
   *
   * @param process a reference to the ProcessHandle object.
   * @param moduleName the name of the module to be created.
   * @return std::expected<Module, memex::Error> a Module object on success, or an error on failure.
   * @note The module name should be the base name (e.g., "kernel32.dll" instead of "C:\Windows\System32\kernel32.dll").
   */
  static std::expected<Module, memex::Error> Create(const ProcessHandle &process, typed::tstring_view moduleName);

  [[nodiscard]]
  /**
   * @brief Getter for the base address of the module.
   *
   * @return RemotePtr<void> The base address of the module.
   */
  RemotePtr<void> base() const;

  [[nodiscard]]
  /**
   * @brief Getter for the size of the module.
   *
   * @return std::size_t The size of the module in bytes.
   */
  std::size_t size() const;

  [[nodiscard]]
  /**
   * @brief Getter for the process' name.
   *
   * @return typed::tstring_view The name of the module.
   */
  typed::tstring_view name() const;

private:
  std::expected<RemotePtr<void>, memex::Error> GetModuleBaseAddress();
};