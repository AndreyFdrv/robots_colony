cmake_minimum_required(VERSION 2.8.3)
project(robots_colony)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  gazebo_ros
  std_msgs
  message_generation)

add_message_files(
   FILES
   Commands.msg
   Coordinates.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES lab5
  CATKIN_DEPENDS gazebo_ros
#  DEPENDS system_lib
)

foreach(dir queen/meshes queen/urdf worker/meshes worker/urdf)
   install(DIRECTORY ${dir}/
      DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
  ${SDFormat_INCLUDE_DIRS}
)

add_executable(worker src/worker.cpp)
add_executable(queen src/queen.cpp)

target_link_libraries(worker
  ${catkin_LIBRARIES}
)
target_link_libraries(queen
  ${catkin_LIBRARIES}
)


