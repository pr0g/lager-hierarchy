#include "hierarchy.h"

#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>

#include <deque>
#include <iostream>

namespace hy {
  model_t update(model_t current, action_t action) {
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

  std::optional<action_t> intent(char event) {
    switch (event) {
      case '+':
        return add_entity_action_t{};
      default:
        return std::nullopt;
    }
  }

  void draw(model_t curr) {
    std::deque<thh::handle_t> entity_handle_stack;
    for (auto it = curr.root_.rbegin(); it != curr.root_.rend(); ++it) {
      entity_handle_stack.push_front(*it);
    }

    struct indent_tracker_t {
      int indent_ = 0;
      int count_ = 0;
    };

    std::deque<indent_tracker_t> indent_tracker;
    indent_tracker.push_front(
      indent_tracker_t{0, (int)entity_handle_stack.size()});

    int level = 0; // the level (row) in the hierarchy
    int last_indent = 0; // most recent indent (col)
    while (!entity_handle_stack.empty()) {
      const auto curr_indent = indent_tracker.front().indent_;

      int last = last_indent;
      while (curr_indent < last) {
        // scope_exit();
        last--;
      }

      const bool last_element = [&] {
        auto& indent_ref = indent_tracker.front();
        indent_ref.count_--;
        if (indent_ref.count_ == 0) {
          indent_tracker.pop_front();
          return true;
        }
        return false;
      }();

      for (const auto ind : indent_tracker) {
        if (ind.count_ != 0 && ind.indent_ != curr_indent) {
          // display_connection(level, ind.indent_);
        }
      }

      const auto entity_handle = entity_handle_stack.front();
      entity_handle_stack.pop_front();

      curr.entities_.call(entity_handle, [&](const auto& entity) {
        const auto& children = entity.children_;

        // display_info_t display_info;
        // display_info.level = level;
        // display_info.indent = curr_indent;
        // display_info.entity_handle = entity_handle;
        // display_info.selected = interaction.selected_ == entity_handle;
        // display_info.collapsed =
        //   interaction.is_collapsed(entity_handle) && !children.empty();
        // display_info.has_children = !children.empty();
        // display_info.name = entity.name_;
        // display_info.last = last_element;

        // display(display_info);
        std::cout << entity.name_ << "\n";

        if (!children.empty() /*&& !interaction.is_collapsed(entity_handle)*/) {
          indent_tracker.push_front(
            indent_tracker_t{curr_indent + 1, (int)children.size()});
          for (auto it = children.rbegin(); it != children.rend(); ++it) {
            entity_handle_stack.push_front(*it);
          }
        }
      });
      level++;
      last_indent = curr_indent;
    }

    while (last_indent > 0) {
      // scope_exit();
      last_indent--;
    }
  }

  void run() {
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
} // namespace hy
