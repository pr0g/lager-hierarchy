#pragma once

#include <thh_handles/thh_handles.hpp>

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace hy {
  struct entity_t {
    entity_t() = default;
    explicit entity_t(std::string name) : name_(std::move(name)) {}
    std::string name_;
    std::vector<thh::handle_t> children_;
    thh::handle_t parent_;
  };

  struct model_t {
    thh::container_t<entity_t> entities_;
    std::vector<thh::handle_t> root_;
  };

  struct add_entity_action_t {};

  using action_t = std::variant<add_entity_action_t>;

  model_t update(model_t current, action_t action);
  std::optional<action_t> intent(char event);
  void draw(model_t curr);

  void run();
} // namespace hy
