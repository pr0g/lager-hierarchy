#include "hierarchy.h"

#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>

#include <deque>
#include <iostream>

namespace la {
  hy::model_t update(hy::model_t current, action_t action) {
    const auto initial_selection = [&] {
      if (
        current.interaction_.element_ == -1
        && current.interaction_.selected_ == thh::handle_t()) {
        if (!current.root_.empty() && !current.entities_.empty()) {
          current.interaction_.element_ = 0;
          current.interaction_.selected_ =
            current.root_[current.interaction_.element_];
          current.interaction_.siblings_ = current.root_;
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
          if (
            current.interaction_.element_ == -1
            && current.interaction_.selected_ == thh::handle_t()) {
            current.root_.push_back(handle);
            current.interaction_.siblings_ =
              hy::siblings(handle, current.entities_, current.root_);
          } else {
            hy::add_children(
              current.interaction_.selected_, {handle}, current.entities_);
            current.interaction_.siblings_ = hy::siblings(
              current.interaction_.selected_, current.entities_, current.root_);
          }
          return current;
        },
        [&](move_down_action_t) {
          if (!initial_selection()) {
            hy::move_down(
              current.interaction_, current.entities_, current.root_);
          }
          return current;
        },
        [&](move_up_action_t) {
          if (!initial_selection()) {
            hy::move_up(current.interaction_, current.entities_, current.root_);
          }
          return current;
        },
        [&](deselect_action_t) {
          current.interaction_.element_ = -1;
          current.interaction_.selected_ = thh::handle_t();
          current.interaction_.siblings_ = {};
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
