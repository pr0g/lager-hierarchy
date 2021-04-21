#include "hierarchy.h"

#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>

#include <deque>
#include <iostream>

namespace la {
  hy::model_t update(hy::model_t current, action_t action) {
    return std::visit(
      lager::visitor{[&](add_entity_action_t) {
        auto handle = current.entities_.add();
        current.root_.push_back(handle);
        current.entities_.call(handle, [handle](auto& entity) {
          entity.name_ = std::string("entity_") + std::to_string(handle.id_);
        });
        return current;
      }},
      action);
  }

  void run(
    const std::function<void(hy::model_t)> draw_fn,
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
