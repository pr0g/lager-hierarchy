#pragma once

#include <optional>
#include <variant>

namespace demo {
  struct model_t {
    int value = 0;
  };

  struct increment_action_t {};
  struct decrement_action_t {};
  struct reset_action_t {
    int new_value = 0;
  };

  using action_t =
    std::variant<increment_action_t, decrement_action_t, reset_action_t>;

  model_t update(model_t c, action_t action);
  std::optional<action_t> intent(char event);
  void draw(model_t curr);

  void run();
} // namespace demo
