cmake_minimum_required(VERSION 3.25) #CMAKE minimum version

project(EngineProject VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23) #C++ standard mimimum version
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #Better portability (doesn't use g++ compilers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Generate commands output during compilation - file: compile_commands.json

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SDLTTF_VENDORED ON)

add_subdirectory(vendor/SDL3)
add_subdirectory(vendor/SDL_ttf )

add_library(glad vendor/glad/src/glad.c)
target_compile_definitions(glad PRIVATE GLAD_GL_IMPLEMENTATION)
target_include_directories(glad PUBLIC vendor/glad/include vendor/stb)


set(ENGINE_SOURCES
  engine/audio/SoundEffect.cpp
  engine/audio/SoundManager.cpp
  engine/core/Application.cpp
  engine/core/Engine.cpp
  engine/core/Time.cpp
  engine/core/Input.cpp
  engine/core/GamepadManager.cpp
  engine/core/PlayerController.cpp
  engine/core/FpsCounter.cpp
  engine/core/CameraController.cpp
  engine/demo/Demo2DScene.cpp
  engine/demo/Demo2DFixedScene.cpp
  engine/diagnostics/Log.cpp
  engine/events/EventBus.cpp
  engine/events/SDLEventTranslator.cpp
  engine/graphics/Font.cpp
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
  engine/scene/ECSScene.cpp
  engine/scene/GameWorld.cpp
  engine/scene/SceneManager.cpp
  engine/scene/MenuScene.cpp
)

add_executable(
  Game
  game/src/main.cpp
  ${ENGINE_SOURCES}
  vendor/glad/src/glad.c
)

target_include_directories(
  Game
  PRIVATE
  ${CMAKE_SOURCE_DIR}/engine
  vendor/entt
  vendor/SDL_ttf/include
  vendor/stb
  vendor/glad/include
)

target_link_libraries(
  Game
  PRIVATE
  SDL3::SDL3
  SDL3_ttf::SDL3_ttf
  #glad
)

target_compile_definitions(Game PRIVATE GLAD_GL_IMPLEMENTATION)
target_include_directories(Game PRIVATE vendor/glad/include)

if (WIN32)
    # Caminho para a SDL3.dll (gerada em build/windows/vendor/SDL3/)
    set(SDL3_DLL_PATH "${CMAKE_BINARY_DIR}/vendor/SDL3/SDL3.dll")
    # Caminho para a SDL3_ttf.dll (gerada em build/windows/vendor/SDL_ttf/)
    set(SDL3_TTF_DLL_PATH "${CMAKE_BINARY_DIR}/vendor/SDL_ttf/SDL3_ttf.dll")

    # Copiar SDL3.dll se existir
    add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_DLL_PATH}"
            "$<TARGET_FILE_DIR:Game>"
        COMMENT "Copying SDL3.dll to executable directory..."
    )

    # Copiar SDL3_ttf.dll se existir
    add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_TTF_DLL_PATH}"
            "$<TARGET_FILE_DIR:Game>"
        COMMENT "Copying SDL3_ttf.dll to executable directory..."
    )
endif()
