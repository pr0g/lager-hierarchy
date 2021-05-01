#include "demo.h"
#include "hierarchy.h"

#include <locale.h>
#include <ncurses.h>

int main(int argc, char** argv) {
  // enable support for unicode characters
  setlocale(LC_CTYPE, "");

  initscr(); // start curses mode
  cbreak(); // line buffering disabled (respects Ctrl-C to quit)
  keypad(stdscr, true); // enable function keys
  noecho(); // don't echo while we do getch
  curs_set(0); // hide cursor

  const auto display_name = [](const hy::display_info_t& di) {
    mvprintw(
      di.level, di.indent * 4,
      di.last ? "\xE2\x94\x94\xE2\x94\x80\xE2\x94\x80 "
              : "\xE2\x94\x9C\xE2\x94\x80\xE2\x94\x80 ");
    if (di.selected) {
      attron(A_REVERSE);
    }
    if (di.collapsed) {
      attron(A_BOLD);
    }
    printw("%s\n", di.name.c_str());
    attroff(A_REVERSE);
    attroff(A_BOLD);
  };

  const auto display_connection = [](int level, int indent) {
    mvprintw(level, indent * 4, "\xE2\x94\x82");
  };

  auto draw = [display_name, display_connection](hy::model_t curr) {
    clear();

    hy::display_hierarchy(
      curr.entities_, curr.interaction_, curr.root_, display_name, [] {},
      display_connection);

    refresh();
    move(0, 0);
  };

  auto intent = []() -> std::optional<la::action_t> {
    switch (int key = getch(); key) {
      case 10: // enter
        return la::add_entity_action_t{};
      case KEY_UP:
        return la::move_up_action_t{};
      case KEY_DOWN:
        return la::move_down_action_t{};
      case KEY_LEFT:
        return la::collapse_action_t{};
      case KEY_RIGHT:
        return la::expand_action_t{};
      case ' ': // space
        return la::deselect_action_t{};
      default:
        return std::nullopt;
    }
  };

  la::run(draw, intent);

  endwin();

  return 0;
}
