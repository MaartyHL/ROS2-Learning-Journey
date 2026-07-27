cmake_minimum_required(VERSION 3.8)
project(my_robot_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Trouver les dépendances
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Déclarer la génération des interfaces (.msg et .srv)
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Turtle.msg"
  "msg/TurtleArray.msg"
  "srv/CatchTurtle.srv"
)

ament_package()