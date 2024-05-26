cmake_minimum_required(VERSION 3.26)
project(SFMLBird)

set(CMAKE_CXX_STANDARD 20)

set(EXT_PATH ${CMAKE_SOURCE_DIR}/ext)
set(SFML_PATH ${EXT_PATH}/SFML-2.6.X)
set(RES_DIR ${CMAKE_SOURCE_DIR}/res)

set(PROGRAM_SRC_PATH ${CMAKE_SOURCE_DIR}/src)
set(PROGRAM_HEADER_PATH ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${SFML_PATH})
include(${PROGRAM_SRC_PATH}/CMakeLists.txt)
include(${PROGRAM_HEADER_PATH}/CMakeLists.txt)

add_executable(SFMLBird ${PROGRAM_SRC} ${PROGRAM_HEADER})

target_include_directories(SFMLBird PRIVATE ${PROGRAM_HEADER_PATH})
target_include_directories(SFMLBird PRIVATE ${SFML_PATH}/include)

add_custom_command(TARGET SFMLBird POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:SFMLBird> $<TARGET_FILE_DIR:SFMLBird>
                   COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET SFMLBird POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${EXT_PATH}/openal32.dll $<TARGET_FILE_DIR:SFMLBird>
                   COMMAND_EXPAND_LISTS
)

add_custom_target(link_res
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${RES_DIR} $<TARGET_FILE_DIR:SFMLBird>/res
)

add_dependencies(SFMLBird link_res)

target_link_libraries(SFMLBird PRIVATE sfml-window)
target_link_libraries(SFMLBird PRIVATE sfml-system)
target_link_libraries(SFMLBird PRIVATE sfml-audio)
target_link_libraries(SFMLBird PRIVATE sfml-network)
target_link_libraries(SFMLBird PRIVATE sfml-graphics)
