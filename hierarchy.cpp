#include "hierarchy.h"

#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>

#include <deque>
#include <iostream>

namespace la {
  hy::model_t update(hy::model_t current, action_t action) {
    const auto initial_selection = [&] {
      if (current.interaction_.selected() == thh::handle_t()) {
        if (!current.root_.empty() && !current.entities_.empty()) {
          current.interaction_.select(
            current.root_.front(), current.entities_, current.root_);
        }
        return true;
      }
      return false;
    };
    return std::visit(
      lager::visitor{
        [&](add_entity_action_t) {
          auto handle = current.entities_.add();
          current.entities_.call(handle, [handle](auto& entity) {
            entity.name_ = std::string("entity_") + std::to_string(handle.id_);
          });
          if (current.interaction_.selected() == thh::handle_t()) {
            current.root_.push_back(handle);
          } else {
            hy::add_children(
              current.interaction_.selected(), {handle}, current.entities_);
          }
          return current;
        },
        [&](move_down_action_t) {
          if (!initial_selection()) {
            current.interaction_.move_down(current.entities_, current.root_);
          }
          return current;
        },
        [&](move_up_action_t) {
          if (!initial_selection()) {
            current.interaction_.move_up(current.entities_, current.root_);
          }
          return current;
        },
        [&](deselect_action_t) {
          current.interaction_.deselect();
          return current;
        }},
      action);
  }

  void run(
    const std::function<void(hy::model_t)>& draw_fn,
    const std::function<std::optional<action_t>()>& intent_fn) {
    auto store = lager::make_store<action_t>(
      hy::model_t{}, lager::with_manual_event_loop{});

    watch(store, draw_fn);

    for (;;) {
      if (auto act = intent_fn()) {
        store.dispatch(*act);
      }
    }
  }
} // namespace la
