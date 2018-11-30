cmake_minimum_required(VERSION 3.2)

project(bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Threads REQUIRED)

configure_file(version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)
add_executable(${PROJECT_NAME} "main.cpp")
add_executable(${PROJECT_NAME}_test main_test.cpp)
add_library(${PROJECT_NAME}_lib lib.cpp)

set_target_properties(${PROJECT_NAME}_lib ${PROJECT_NAME}
    ${PROJECT_NAME}_test
    PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(${PROJECT_NAME}_test PROPERTIES
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

# for local build
target_link_libraries(${PROJECT_NAME}_lib stdc++)
target_link_libraries(${PROJECT_NAME} stdc++)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME}_test ${Boost_LIBRARIES} ${PROJECT_NAME}_lib)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT stillarty@ya.ru)

include (CPack)

enable_testing()

add_test(test_bulk ${PROJECT_NAME}_test)
