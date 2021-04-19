#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>

#include <iostream>
#include <optional>
#include <variant>

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

model_t update(model_t c, action_t action) {
  return std::visit(
    lager::visitor{
      [&](increment_action_t) {
        ++c.value;
        return c;
      },
      [&](decrement_action_t) {
        --c.value;
        return c;
      },
      [&](reset_action_t a) {
        c.value = a.new_value;
        return c;
      },
    },
    action);
}

std::optional<action_t> intent(char event) {
  switch (event) {
    case '+':
      return increment_action_t{};
    case '-':
      return decrement_action_t{};
    case '.':
      return reset_action_t{};
    default:
      return std::nullopt;
  }
}

void draw(model_t curr) {
  std::cout << "current value: " << curr.value << '\n';
}

int main(int argc, char** argv) {
  auto store =
    lager::make_store<action_t>(model_t{}, lager::with_manual_event_loop{});

  watch(store, draw);

  auto event = char{};
  while (std::cin >> event) {
    if (auto act = intent(event)) {
      store.dispatch(*act);
    }
  }
}
