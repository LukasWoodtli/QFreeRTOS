cmake_minimum_required(VERSION 2.8.11)


project(QFreeRTOS)

set(QFREERTOS_SOURCES)
set(QFREERTOS_HEADERS)
set(QFREERTOS_INCLUDE_DIRS)

include_directories(Source/include
                    Source/portable/Qt
                    ${QFREERTOS_INCLUDE_DIRS})


add_subdirectory(Source)

add_subdirectory(Demo/Qt)
