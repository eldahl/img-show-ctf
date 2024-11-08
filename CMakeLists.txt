cmake_minimum_required(VERSION 3.29)

set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(GLFW_BUILD_STATIC ON)

project(img-show-ctf)


find_package(glfw3 3.4 REQUIRED)
find_package(glm 1.0.1 REQUIRED)

# Creates C resources file from files in given directory

function(create_resources dir output)
  # Create empty output file
  file(WRITE ${output} "")
  # Collect input files
  file(GLOB bins ${dir}/*)
  # Iterate through input files
  foreach(bin ${bins})
    # Get short filename
    string(REGEX MATCH "([^/]+)$" filename ${bin})

    # Ensure filename is not empty
    if(NOT filename)
      message(WARNING "Skipping file ${bin} due to empty filename.")
      continue()
    endif()

    # Replace filename spaces & extension separator for C compatibility
    string(REGEX REPLACE "\\.| |-" "_" filename ${filename})

    # Read hex data from file
    file(READ ${bin} filedata HEX)

    # Check if filedata is valid
    if(NOT filedata)
      message(
        WARNING "Skipping file ${bin} due to empty or unreadable content.")
      continue()
    endif()

    # Convert hex data for C compatibility
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})

    # Append data to output file
    file(
      APPEND ${output}
      "const unsigned char ${filename}[] = {${filedata}};\nconst unsigned ${filename}_size = sizeof(${filename});\n"
    )
  endforeach()
endfunction()

create_resources("${CMAKE_CURRENT_SOURCE_DIR}/res/imgs"
                 "${CMAKE_CURRENT_BINARY_DIR}/generated_res.c")

set(GEN_RES_SOURCE ${CMAKE_BINARY_DIR}/generated_res.c)
message(STATUS "Generated resource file: ${GEN_RES_SOURCE}")

# GLAD files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c
     include/glad/src/glad.c)
set(GLAD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/src/glad.c)

# stb-image.h and future files in include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB INCLUDES include/*.h include/*.cpp)

file(GLOB SOURCES src/*.h src/*.cpp)

IF (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    add_executable(${PROJECT_NAME} WIN32 src/main.cpp ${GLAD_SOURCES} ${INCLUDES}
                               ${CMAKE_CURRENT_BINARY_DIR}/generated_res.c ${SOURCES})
ELSE()
    add_executable(${PROJECT_NAME} src/main.cpp ${GLAD_SOURCES} ${INCLUDES}
                               ${CMAKE_CURRENT_BINARY_DIR}/generated_res.c ${SOURCES})
ENDIF()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# GLFW
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
add_definitions(-DGLFW_STATIC)
