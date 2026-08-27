cmake_minimum_required(VERSION 3.25) #Versão mínima do CMAKE

project(
  EngineProject
  VERSION 0.1.0
  LANGUAGES C CXX
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

add_library(
  glad
  vendor/glad/src/glad.c
)

target_include_directories(
  glad
  PUBLIC
  vendor/glad/include
  vendor/stb
)

add_executable(
  Game
  game/src/main.cpp
  engine/core/Application.cpp
  engine/core/Engine.cpp
  engine/core/Time.cpp
  engine/core/Input.cpp
  engine/core/PlayerController.cpp
  engine/core/FpsCounter.cpp
  engine/core/CameraController.cpp
  engine/diagnostics/Log.cpp
  engine/graphics/IndexBuffer.cpp
  engine/graphics/Shader.cpp
  engine/graphics/VertexArray.cpp
  engine/graphics/VertexBuffer.cpp
  engine/graphics/Texture2D.cpp
  engine/graphics/GraphicsContext.cpp
  engine/math/Matrix4.cpp
  engine/platform/sdl/SDLWindow.cpp
  engine/renderer/Renderer.cpp
  engine/resources/ResourceManager.cpp
  engine/scene/Camera2D.cpp
  engine/scene/Scene.cpp
)

target_include_directories(
  Game
  PRIVATE
  engine
  vendor/entt
)

target_link_libraries(
  Game
  PRIVATE
  SDL3::SDL3
  glad
)
