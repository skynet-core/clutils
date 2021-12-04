cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE
    $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "")
message(STATUS ${CMAKE_TOOLCHAIN_FILE})

set(clutils_VERSION 0.1.3)

project(
  clutils
  VERSION ${clutils_VERSION}
  DESCRIPTION "A simple OpenCL helpers"
  HOMEPAGE_URL "https://github.com/skynet-core/clutils"
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)
include(CPack)

add_library(clutils src/clutils.cpp include/clutils/clutils.h)
add_library(clutils::clutils ALIAS clutils)

find_package(OpenCL REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(clutils PRIVATE OpenCL::OpenCL fmt::fmt-header-only)

if(BUILD_SHARED_LIBS AND WIN32)
  target_compile_definitions(
    clutils
    PRIVATE "CLUTILS_EXPORT=__declspec(dllexport)"
    INTERFACE "CLUTILS_EXPORT=__declspec(dllimport)")
endif()

target_compile_features(clutils PUBLIC cxx_std_17)
target_include_directories(
  clutils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(BUILD_TESTING AND CLUTILS_BUILD_TESTS)
  add_subdirectory(tests)
  add_test(NAME TestMain COMMAND ${PROJECT_NAME}_test)
endif()

configure_package_config_file(
  cmake/config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/clutils-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/clutils
  NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/clutils-config-version.cmake
  VERSION ${beicode_VERSION}
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/clutils-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/clutils-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/clutils)

install(TARGETS clutils EXPORT clutils-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
  EXPORT clutils-targets
  NAMESPACE clutils::
  DESTINATION ${CMAKE_INSTALL_DATADIR}/clutils)
