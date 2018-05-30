cmake_minimum_required(VERSION 2.8.3)
project(mypackage)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  cv_bridge
  sensor_msgs
  image_transport
  socketcan_interface
  socketcan_bridge
        )


find_package(OpenCV REQUIRED)

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES mypackage
        CATKIN_DEPENDS roscpp std_msgs cv_bridge sensor_msgs image_transport
        DEPENDS system_lib
)
include_directories(
 include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node bambam.cpp src/publishersubscriber.cpp)

target_link_libraries( ${PROJECT_NAME}_node ${OpenCV_LIBS} ${catkin_LIBRARIES})  #EOL                          )i


install(
        TARGETS
        ${PROJECT_NAME}_node
        # //add more stuff in here
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
