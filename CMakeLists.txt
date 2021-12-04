cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE
    $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "")
message(STATUS ${CMAKE_TOOLCHAIN_FILE})

project(
  clutils
  VERSION 0.1.2
  LANGUAGES CXX)
include(GNUInstallDirs)
include(CTest)

file(GLOB ${PROJECT_NAME}_SOURCES src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(${PROJECT_NAME} "${${PROJECT_NAME}_SOURCES}")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL
                                              fmt::fmt-header-only)

# dependencies
find_package(OpenCL REQUIRED)
find_package(fmt CONFIG REQUIRED)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(${PROJECT_NAME}_INSTALL_LIBRARY
       "Enable installing of ${PROJECT_NAME} library into default locations"
       ${IS_TOPLEVEL_PROJECT})
option(${PROJECT_NAME}_BUILD_TESTING "Build and run ${PROJECT_NAME} tests "
       ${IS_TOPLEVEL_PROJECT})
option(${PROJECT_NAME}_BUILD_DOC "Generate ${PROJECT_NAME} documentation"
       ${IS_TOPLEVEL_PROJECT})

if(BUILD_TESTING AND ${PROJECT_NAME}_BUILD_TESTING)
  add_subdirectory(tests)
  add_test(NAME TestMain COMMAND ${PROJECT_NAME}_test)
endif()

if(${PROJECT_NAME}_INSTALL_LIBRARY)
  # locations are provided by GNUInstallDirs
  message(STATUS "Generate library targets")
  install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)
