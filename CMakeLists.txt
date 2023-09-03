cmake_minimum_required(VERSION 3.10)
project(opencltest VERSION 0.1.0)

add_executable(${PROJECT_NAME}
    main.cpp
    OpenClBlackbox.cpp
    Game.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/OpenCL/common/inc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/OpenCL/common/lib/x64/OpenCL.lib
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
