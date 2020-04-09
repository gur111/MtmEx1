cmake_minimum_required(VERSION 3.0.0)
project(VsCodeCMakeTets VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()
# set(CMAKE_TEST_DEBUG "-std=c99 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_DEBUG "-std=c99 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${CMAKE_TEST_DEBUG})


add_executable(VsCodeCMakeTests main.c)
add_executable(Q1Tester Dry/q1Tester.c Dry/q1.c)
# target_compile_options(VsCodeCMakeTets PUBLIC "$<$<CONFIG:DEBUG>:${CMAKE_TEST_DEBUG}>")
# target_compile_options(VsCodeCMakeTets PUBLIC "$<$<CONFIG:RELEASE>:${CMAKE_TEST_RELEASE}>")

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
