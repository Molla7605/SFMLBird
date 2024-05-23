cmake_minimum_required(VERSION 3.11)
project(SFMLBird)

set(EXT_PATH ${CMAKE_SOURCE_DIR}/ext)
set(SFML_PATH ${EXT_PATH}/SFML-2.6.X)

set(PROGRAM_SRC_PATH ${CMAKE_SOURCE_DIR}/src)
set(PROGRAM_HEADER_PATH ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${SFML_PATH})
include(${PROGRAM_SRC_PATH}/CMakeLists.txt)
include(${PROGRAM_HEADER_PATH}/CMakeLists.txt)

add_executable(SFMLBird ${PROGRAM_SRC} ${PROGRAM_HEADER})

target_include_directories(SFMLBird PUBLIC ${PROGRAM_HEADER_PATH})
target_include_directories(SFMLBird PUBLIC ${SFML_PATH}/include)

target_compile_features(SFMLBird PUBLIC cxx_std_20)

target_link_libraries(SFMLBird PUBLIC sfml-window)
target_link_libraries(SFMLBird PUBLIC sfml-system)
target_link_libraries(SFMLBird PUBLIC sfml-audio)
target_link_libraries(SFMLBird PUBLIC sfml-network)
target_link_libraries(SFMLBird PUBLIC sfml-graphics)
