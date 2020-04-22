cmake_minimum_required(VERSION 3.0.0)
project(VsCodeCMakeTets VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()
# set(CMAKE_TEST_DEBUG "-std=c99 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_DEBUG "-std=c99 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${CMAKE_TEST_DEBUG})
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -m64")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -arch x86_64 -m64")
#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -arch x86_64 -m64")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -arch x86_64 -m64")
#set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -arch x86_64 -m64")

add_executable(Q1Tester Dry/q1Tester.c Dry/q1.c)
add_executable(MapTester mtm_map/tests/map_tests.c mtm_map/map.c)

#link_directories(.)
add_executable(ElectionTester election/election.c mtm_map/map.c election/augmented_map.c)
#target_link_libraries(ElectionTester libmap.a)

add_executable(Q2Tester Dry/q2.c)
# target_compile_options(VsCodeCMakeTets PUBLIC "$<$<CONFIG:DEBUG>:${CMAKE_TEST_DEBUG}>")
# target_compile_options(VsCodeCMakeTets PUBLIC "$<$<CONFIG:RELEASE>:${CMAKE_TEST_RELEASE}>")

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
