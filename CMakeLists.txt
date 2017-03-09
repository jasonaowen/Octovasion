cmake_minimum_required (VERSION 3.2)
project (Octovasion CXX)
add_executable (Octovasion src/main.cpp
                           src/GameState.cpp
                           src/GameState.hpp
                    )
add_executable (Test test/catch.cpp
                     src/GameState.cpp
                     src/GameState.hpp
                     test/GameStateTests.cpp
                     )
target_compile_features (Octovasion PRIVATE cxx_generalized_initializers
                                            cxx_range_for
                                            cxx_strong_enums)
target_compile_features (Test PRIVATE cxx_generalized_initializers
                                      cxx_range_for
                                      cxx_strong_enums)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Octovasion_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(Octovasion ${SDL2_LIBRARY}
                                 ${SDL2_IMAGE_LIBRARIES})

include (ExternalProject)
find_package (Git REQUIRED)
ExternalProject_ADD (
  catch
  PREFIX ${CMAKE_BINARY_DIR}/catch
  GIT_REPOSITORY https://github.com/philsquared/Catch.git
  TIMEOUT 10
  UPDATE_COMMAND ${GIT_EXECUTABLE} pull
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
)
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)
add_test(NAME catch
         COMMAND Test)
