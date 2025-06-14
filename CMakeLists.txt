cmake_minimum_required(VERSION 3.10)

project(CSVLevelLoader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/MP)
endif()

#Create some QoL vars
set(SRC_DIR "${CMAKE_SOURCE_DIR}/Hornet")
set(INCLUDE_DIR "${SRC_DIR}/include")
set(SDL2_LIB_DIR "${SRC_DIR}/lib/x64")

#Specific source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SRC_DIR}/Engine/*.cpp" "${SRC_DIR}/Engine/*.h"
    "${SRC_DIR}/GameObjects/*.cpp" "${SRC_DIR}/GameObjects/*.h"
    "${SRC_DIR}/Layers/*.cpp" "${SRC_DIR}/Layers/*.h"
    "${SRC_DIR}/src/*.cpp" "${SRC_DIR}/src/*.h"
)

add_executable(CSVLevelLoader ${SOURCES})

#SDL includes
target_include_directories(CSVLevelLoader PUBLIC
    ${INCLUDE_DIR}
)

#SDL lib dir
link_directories(${SDL2_LIB_DIR})

#Link SDL's libs
target_link_libraries(CSVLevelLoader PRIVATE
    "${SDL2_LIB_DIR}/SDL2.lib"
    "${SDL2_LIB_DIR}/SDL2main.lib"
    "${SDL2_LIB_DIR}/SDL2_image.lib"
    "${SDL2_LIB_DIR}/SDL2_mixer.lib"
    "${SDL2_LIB_DIR}/SDL2_ttf.lib"
    "${SDL2_LIB_DIR}/SDL2test.lib"
)

set(PROGRAM_FOLDER "${SRC_DIR}/Program")

# Copy program
add_custom_command(TARGET CSVLevelLoader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROGRAM_FOLDER}"
    "$<TARGET_FILE_DIR:CSVLevelLoader>/Program"
)

# Copy DLLs into root
file(GLOB SDL_DLLS "${PROGRAM_FOLDER}/*.dll")
foreach(dll ${SDL_DLLS})
    add_custom_command(TARGET CSVLevelLoader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${dll}"
        "$<TARGET_FILE_DIR:CSVLevelLoader>"
    )
endforeach()