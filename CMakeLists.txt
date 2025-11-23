cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(termite VERSION 0.0.1 LANGUAGES CXX)

include(CMakeDependentOption)
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(termite INTERFACE)
target_include_directories(termite INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>
)

if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  add_library(termite::termite ALIAS termite)
endif()

option(TERMITE_BUILD_PACKAGE "Build package files as well" ON)

cmake_dependent_option(TERMITE_BUILD_TEST "Enable termite test" ON
"BUILD_TESTING" OFF)

if(TERMITE_BUILD_TEST)
  include(FetchContent)

  set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
  set(CATCH_INSTALL_HELPERS OFF)
  set(CATCH_BUILD_TESTING OFF)
  set(CATCH_INSTALL_DOCS OFF)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1 # or a later release
  )
  FetchContent_MakeAvailable(Catch2)

  add_executable(${PROJECT_NAME}_test "test/expected_test.cpp")
  target_link_libraries(${PROJECT_NAME}_test PRIVATE
  Catch2::Catch2WithMain termite)

  add_test(NAME termite::test COMMAND ${PROJECT_NAME}_test)
endif()

if(TERMITE_BUILD_PACKAGE)
  include(CMakePackageConfigHelpers)
  include(GNUInstallDirs)

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}_config.cmake.in"
    "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}_config.cmake"
    INSTALL_DESTINATION "share/cmake/${PROJECT_NAME}"
  )

  write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}_config_version.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
  )

  install(TARGETS termite
    EXPORT ${PROJECT_NAME}_targets
    INCLUDES DESTINATION "${CMAKE_INSTALL_DATADIR}"
  )

  install(EXPORT ${PROJECT_NAME}_targets
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
    NAMESPACE termite::
    FILE "${PROJECT_NAME}_targets.cmake"
  )

  install(FILES
    "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}_config.cmake"
    "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}_config_version.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
  )

  install(DIRECTORY "include/" TYPE INCLUDE)

  list(APPEND sourceGenerators TBZ2 TGZ TXZ ZIP)
  list(APPEND binaryGenerators "DEB")

  set(CPACK_SOURCE_GENERATOR ${sourceGenerators})
  set(CPACK_GENERATOR ${binaryGenerators})
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}")
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "obsidian")
  list(APPEND CPACK_SOURCE_IGNORE_FILES /.git/ /build/ .gitignore)

  include(CPack)
endif()

