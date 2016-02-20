cmake_minimum_required(VERSION 2.8.11)

project(QFreeRTOS)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Core)


set(SOURCES
  main_blinky.cpp
  mainclass.cpp
)  

set(HEADERS
  FreeRTOSConfig.h
  mainclass.h
)

add_subdirectory(../../Source bin/)

include_directories(../../Source/include
                    ../../Source/portable/Qt

)


# Tell CMake to create the helloworld executable
add_executable(qfreertos ${SOURCES} ${HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(qfreertos Qt5::Core)
