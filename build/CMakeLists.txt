cmake_minimum_required(VERSION 3.10)

project(RANDOM_BAB VERSION 1.0)

if(CMAKE_SYSTEM MATCHES Windows)
  message(STATUS "Target system is Windows")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # set(CMAKE_CXX_STANDARD 23)
  set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)
  set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../inc)

  add_executable(Random_Bab ${SRC_DIR}/main.c ${SRC_DIR}/array.c
                            ${SRC_DIR}/slack_api.c ${SRC_DIR}/shuffle.c)

  target_include_directories(Random_Bab PRIVATE ${INCLUDE_DIR})
  target_link_libraries(Random_Bab cJSON curl)
endif()

if(CMAKE_SYSTEM MATCHES Linux)
  message(STATUS "Target system is Linux")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # set(CMAKE_CXX_STANDARD 23)
  set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)
  set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../inc)
  set(LIB_DIR /usr/local/lib)

  add_executable(Random_Bab ${SRC_DIR}/main.c ${SRC_DIR}/array.c
                            ${SRC_DIR}/slack_api.c ${SRC_DIR}/shuffle.c)

  target_include_directories(Tutorial PRIVATE ${INCLUDE_DIR} /opt/local/include)
  target_include_directories(Random_Bab PRIVATE ${INCLUDE_DIR})
  target_link_libraries(Random_Bab cJSON curl)
endif()

if(CMAKE_SYSTEM MATCHES Darwin)
  message(STATUS "Target system is MacOS")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # set(CMAKE_CXX_STANDARD 23)
  set(SRC_DIR ${CMAKE_SOURCE_DIR}/../src)
  set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../inc)
  set(LIB_DIR /opt/homebrew/lib)

  add_executable(Random_Bab ${SRC_DIR}/main.c ${SRC_DIR}/array.c
                            ${SRC_DIR}/slack_api.c ${SRC_DIR}/shuffle.c)

  target_include_directories(Random_Bab PRIVATE ${INCLUDE_DIR}
                                                /opt/homebrew/include)

  target_link_directories(Random_Bab PRIVATE ${LIB_DIR})
  target_link_libraries(Random_Bab cJSON curl)
endif()
