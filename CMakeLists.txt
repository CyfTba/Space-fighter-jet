cmake_minimum_required(VERSION 3.10)
project(SPACE_FIGHTER_JET)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

#根据操作系统设置文件名称
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

include_directories(${CMAKE_SOURCE_DIR}/SDL2/include/SDL2/)
link_directories(${CMAKE_SOURCE_DIR}/SDL2/lib/)

add_executable(${TARGET} ${SRC} "resources.rc")

target_link_libraries(${TARGET} SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)