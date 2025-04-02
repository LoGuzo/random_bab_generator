cmake_minimum_required(VERSION 3.10)
project(RANDOM_BAB VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../inc)

set(SOURCES ${SRC_DIR}/main.c ${SRC_DIR}/array.c ${SRC_DIR}/slack_api.c
            ${SRC_DIR}/shuffle.c)

# Detect target architecture for cross-compilation
if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
  execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

message(STATUS "Target OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Target Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "Compiling for Windows")

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL
                                                "x86_64")
    set(LIB_DIR "C:/libs/x86_64") # Default Windows x86_64 lib path
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64" OR CMAKE_SYSTEM_PROCESSOR
                                                    STREQUAL "aarch64")
    set(LIB_DIR "C:/libs/arm64") # Windows on ARM64
  else()
    message(
      FATAL_ERROR "Unsupported Windows architecture: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()

  # Windows does not have a SECOND_INCLUDE_DIR
  set(SECOND_INCLUDE_DIR "")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Compiling for Linux")

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(LIB_DIR "/usr/lib/x86_64-linux-gnu/")
    set(SECOND_INCLUDE_DIR "/usr/include/x86_64-linux-gnu/")
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(LIB_DIR "/usr/lib/aarch64-linux-gnu/")
    set(SECOND_INCLUDE_DIR "/usr/include/aarch64-linux-gnu/")
  else()
    message(
      FATAL_ERROR "Unsupported Linux architecture: ${CMAKE_SYSTEM_PROCESSOR}")
  endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message(STATUS "Compiling for macOS")

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(LIB_DIR "/opt/homebrew/lib")
    set(SECOND_INCLUDE_DIR "/opt/homebrew/include")
  else()
    set(LIB_DIR "/usr/local/lib")
    set(SECOND_INCLUDE_DIR "/usr/local/include")
  endif()

else()
  message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

# Build the executable
add_executable(Random_Bab ${SOURCES})

# Include only necessary directories
if(SECOND_INCLUDE_DIR)
  target_include_directories(Random_Bab PRIVATE ${INCLUDE_DIR}
                                                ${SECOND_INCLUDE_DIR})
else()
  target_include_directories(Random_Bab PRIVATE ${INCLUDE_DIR})
endif()

target_link_directories(Random_Bab PRIVATE ${LIB_DIR})
target_link_libraries(Random_Bab jansson curl)
