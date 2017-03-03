cmake_minimum_required (VERSION 3.2)
project (GIP CXX)
add_executable (GIP src/main.cpp
                    )
add_executable (Test test/catch.cpp
                     test/example.cpp
                     )
target_compile_features (GIP PRIVATE cxx_generalized_initializers
                                     cxx_range_for
                                     cxx_strong_enums)
target_compile_features (Test PRIVATE cxx_generalized_initializers
                                      cxx_range_for
                                      cxx_strong_enums)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GIP_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(GIP ${SDL2_LIBRARY}
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
