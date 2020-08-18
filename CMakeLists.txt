cmake_minimum_required(VERSION 3.0.2)
add_compile_options(-std=c++17)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost 1.45.0 REQUIRED COMPONENTS system)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(astar ./astar.cc)

target_link_libraries(astar ${OpenCV_LIBS} ${Boost_LIBRARIES})