# Lager-Hierarchy

Experimental application using the [lager](https://github.com/arximboldi/lager) library to display the [hierarchy](https://github.com/pr0g/hierarchy) example.

## Dependencies

The example application is currently only tested on macOS using `ncurses`. It also depends on [Boost Hana](https://www.boost.org/doc/libs/1_61_0/libs/hana/doc/html/index.html). All other dependencies (including `lager`, `immer` and `zug` are brought in with `FetchContent` in CMake).

The `configure.sh` file serves as an example with various CMake variables configured to not build the examples and tests of the dependencies.
