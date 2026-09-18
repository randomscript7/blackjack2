cmake_minimum_required(VERSION 3.21)
project(sasha LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(sfml)

# ponytail: only main.cpp — it #includes the other .cpps, adding them too causes duplicate symbols
add_executable(sasha BlackJack-2/main.cpp)
target_link_libraries(sasha PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

if(NOT WIN32)
  # ponytail: build-only Sleep shim; real windows.h wins on Windows (dir never added there)
  target_include_directories(sasha PRIVATE compat)
endif()

# ponytail: code loads assets via CWD-relative paths, so copy them next to the exe
set(SASHA_ASSETS
  backdrop.png
  dice.png
  greenbutton.png
  redbutton.png
  helpButton.png
  mute.png
  PressStart2P.ttf
  Balatro-sound.ogg
)
foreach(f IN LISTS SASHA_ASSETS)
  add_custom_command(TARGET sasha POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/BlackJack-2/${f} $<TARGET_FILE_DIR:sasha>)
endforeach()
add_custom_command(TARGET sasha POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/BlackJack-2/cards $<TARGET_FILE_DIR:sasha>/cards)

if(WIN32)
  add_custom_command(TARGET sasha POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:sasha> $<TARGET_FILE_DIR:sasha>
    COMMAND_EXPAND_LISTS)
  # ponytail: sfml-audio loads OpenAL at runtime but it isn't a CMake target, so RUNTIME_DLLS misses it
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SASHA_OPENAL_DIR ${sfml_SOURCE_DIR}/extlibs/bin/x64)
  else()
    set(SASHA_OPENAL_DIR ${sfml_SOURCE_DIR}/extlibs/bin/x86)
  endif()
  add_custom_command(TARGET sasha POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${SASHA_OPENAL_DIR}/openal32.dll $<TARGET_FILE_DIR:sasha>)
endif()

set_target_properties(sasha PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:sasha>)
