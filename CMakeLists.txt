cmake_minimum_required(VERSION 3.15)
project(lager-test)

include(FetchContent)
FetchContent_Declare(
  thh_handles
  SOURCE_DIR ../../cpp-handles)
  # GIT_REPOSITORY https://github.com/pr0g/cpp-handle-container.git
  # GIT_TAG        de5dfa1234d81f0054d02347567d5bf949531be5)
FetchContent_Declare(
  zug
  GIT_REPOSITORY https://github.com/arximboldi/zug.git
  GIT_TAG b28a04ce6ed457dd9cff2232b344499b1b48e884)
FetchContent_Declare(
  lager
  GIT_REPOSITORY https://github.com/arximboldi/lager.git
  GIT_TAG c9d8b7d3c7dc7138913757d1624ab705866d791d)
FetchContent_Declare(
  immer
  GIT_REPOSITORY https://github.com/arximboldi/immer.git
  GIT_TAG cb054e982d202b72d3843fb0b18a15e62221ba33)
FetchContent_Declare(
  hierarchy
  SOURCE_DIR ../../hierarchy)
  # GIT_REPOSITORY https://github.com/pr0g/hierarchy.git
  # GIT_TAG c9df14a8dc440834f20ea6d9cb893940f1be10af)
FetchContent_MakeAvailable(lager zug thh_handles immer hierarchy)

find_package(Hana REQUIRED)
find_package(Curses REQUIRED)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp demo.cpp hierarchy.cpp)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE lager zug hana thh_handles immer hierarchy Curses)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
