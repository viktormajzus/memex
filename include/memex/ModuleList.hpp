#pragma once
#include <memex/Module.hpp>
#include <memex/error.hpp>
#include <memex/typed.hpp>

#include <vector>
#include <expected>
#include <optional>


class ModuleList
{
private:
  const ProcessHandle* m_process{};
  std::vector<Module> m_modules{};

  explicit ModuleList(const ProcessHandle& process);

public:
  static std::expected<ModuleList, memex::Error> Create(const ProcessHandle& process);

  std::expected<void, memex::Error> refresh();

  std::optional<Module> find(typed::tstring_view name) const;
  std::optional<Module> find(RemotePtr<void> address) const;

  [[nodiscard]]
  const std::vector<Module>& all() const;

  [[nodiscard]]
  std::size_t size() const;

  void clear();

  bool empty() const;

  bool contains(typed::tstring_view name) const;
  bool contains(RemotePtr<void> address) const;

  std::optional<Module> findWithinRange(RemotePtr<void> address) const;
};
