cmake_minimum_required(VERSION 3.25) #Versão mínima do CMAKE

project(
  EngineProject
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23) #Versão mínima standard C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #Melhor portabilidade (não usa extensões gnu++)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Gera output dos comandos na compilação - ficheiro compile_commands.json

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
  )
endif()

add_subdirectory(vendor/SDL3)

add_executable(
  Game
  game/src/main.cpp
)

target_link_libraries(
  Game
  PRIVATE
  SDL3::SDL3
)
