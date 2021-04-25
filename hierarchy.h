#pragma once

#include <hierarchy/entity.hpp>

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace la {
  struct add_entity_action_t {};
  struct move_down_action_t {};
  struct move_up_action_t {};
  struct deselect_action_t {};

  using action_t = std::variant<
    add_entity_action_t, move_down_action_t, move_up_action_t,
    deselect_action_t>;

  hy::model_t update(hy::model_t current, action_t action);

  void run(
    const std::function<void(hy::model_t)>& draw_fn,
    const std::function<std::optional<action_t>()>& intent_fn);
} // namespace la
