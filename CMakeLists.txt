cmake_minimum_required(VERSION 3.9)

project(sheeplib VERSION 1.0.0 DESCRIPTION "general library for C99")

include(GNUInstallDirs)

FILE(GLOB SOURCES src/*.c)
file(GLOB HEADERS include/*.h)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)


install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})

install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
