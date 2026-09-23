cmake_minimum_required(VERSION 3.25) #CMAKE minimum version

project(EngineProject VERSION 0.1.0 LANGUAGES C CXX)

#C++ configuration
set(CMAKE_CXX_STANDARD 23) #C++ standard mimimum version
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #Better portability (doesn't use g++ compilers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Generate commands output during compilation - file: compile_commands.json

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#Vendored dependencies
set(SDLTTF_VENDORED ON)

add_subdirectory(vendor/SDL3)
add_subdirectory(vendor/SDL_ttf )

add_library(glad vendor/glad/src/glad.c)
target_compile_definitions(glad PRIVATE GLAD_GL_IMPLEMENTATION)
target_include_directories(glad PUBLIC vendor/glad/include vendor/stb)

#Engine sources
set(ENGINE_SOURCES
  #Audio
  engine/audio/SoundEffect.cpp
  engine/audio/SoundManager.cpp

  #Core
  engine/core/Application.cpp
  engine/core/Engine.cpp
  engine/core/Time.cpp
  engine/core/Input.cpp
  engine/core/GamepadManager.cpp
  engine/core/PlayerController.cpp
  engine/core/FpsCounter.cpp
  engine/core/CameraController.cpp

  #Demo Scenes
  engine/demo/Demo2DScene.cpp
  engine/demo/Demo2DFixedScene.cpp
  engine/demo/levels/Level1.cpp
  engine/demo/levels/Level2.cpp
  engine/demo/levels/Level3.cpp

  #Diagnostics/Logging
  engine/diagnostics/Log.cpp

  #Events
  engine/events/EventBus.cpp
  engine/events/SDLEventTranslator.cpp

  #Graphics
  engine/graphics/Font.cpp
  engine/graphics/IndexBuffer.cpp
  engine/graphics/Shader.cpp
  engine/graphics/VertexArray.cpp
  engine/graphics/VertexBuffer.cpp
  engine/graphics/Texture2D.cpp
  engine/graphics/GraphicsContext.cpp

  #Level System
  engine/level/LevelRegistry.cpp
  engine/level/LevelSelectorScene.cpp

  #Math
  engine/math/Matrix4.cpp

  #Platform
  engine/platform/sdl/SDLWindow.cpp

  #Renderer
  engine/renderer/Renderer.cpp

  #Resources
  engine/resources/ResourceManager.cpp

  #Scene
  engine/scene/Camera2D.cpp
  engine/scene/ECSScene.cpp
  engine/scene/GameWorld.cpp
  engine/scene/SceneManager.cpp
  engine/scene/MenuScene.cpp
  engine/scene/BaseLevelScene.cpp
  engine/scene/HUD.cpp

  #Systems
  engine/systems/LevelSystem.cpp
)

#Executable
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
)

target_compile_definitions(Game PRIVATE GLAD_GL_IMPLEMENTATION)
target_include_directories(Game PRIVATE vendor/glad/include)

#Copy runtime DLLs to executable directory(Windows Only)
if (WIN32)
    set(SDL3_DLL_PATH "${CMAKE_BINARY_DIR}/vendor/SDL3/SDL3.dll")
    set(SDL3_TTF_DLL_PATH "${CMAKE_BINARY_DIR}/vendor/SDL_ttf/SDL3_ttf.dll")

    add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_DLL_PATH}"
            "$<TARGET_FILE_DIR:Game>"
        COMMENT "Copying SDL3.dll to executable directory..."
    )

    add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_TTF_DLL_PATH}"
            "$<TARGET_FILE_DIR:Game>"
        COMMENT "Copying SDL3_ttf.dll to executable directory..."
    )
endif()

#Generate build scripts with logging
#These scripts run Ninja and tee output to build.log.
#Generated automatically on each CMake configure.
if(WIN32)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/build_logged.bat.in
        ${CMAKE_BINARY_DIR}/build_logged.bat
        @ONLY
    )
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/build_logged.ps1.in
        ${CMAKE_BINARY_DIR}/build_logged.ps1
        @ONLY
    )
    message(STATUS "Generated: ${CMAKE_BINARY_DIR}/build_logged.bat")
    message(STATUS "Generated: ${CMAKE_BINARY_DIR}/build_logged.ps1")
else()
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/build_logged.sh.in
        ${CMAKE_BINARY_DIR}/build_logged.sh
        @ONLY
    )
    execute_process(
        COMMAND chmod +x ${CMAKE_BINARY_DIR}/build_logged.sh
    )
    message(STATUS "Generated: ${CMAKE_BINARY_DIR}/build_logged.sh")
endif()
