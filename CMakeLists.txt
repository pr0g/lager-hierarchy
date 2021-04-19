cmake_minimum_required(VERSION 3.15)
project(larger-test)

include(FetchContent)
FetchContent_Declare(
  zug
  GIT_REPOSITORY https://github.com/arximboldi/zug.git
  GIT_TAG b28a04ce6ed457dd9cff2232b344499b1b48e884)
FetchContent_Declare(
  lager
  GIT_REPOSITORY https://github.com/arximboldi/lager.git
  GIT_TAG c9d8b7d3c7dc7138913757d1624ab705866d791d)
FetchContent_MakeAvailable(lager zug)

find_package(Hana REQUIRED)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lager zug hana)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
