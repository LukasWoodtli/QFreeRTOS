
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/portable/Qt)


set(QFREERTOS_SOURCES
    ${QFREERTOS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/portable/MemMang/heap_2.c
    PARENT_SCOPE
)

set(QFREERTOS_HEADERS
    ${QFREERTOS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/FreeRTOS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/StackMacros.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/croutine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/deprecated_definitions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/event_groups.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mpu_wrappers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/portable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/projdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/queue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/semphr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/task.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/timers.h
    PARENT_SCOPE
)

set(QFREERTOS_INCLUDE_DIRS
    ${QFREERTOS_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}
   PARENT_SCOPE
)

