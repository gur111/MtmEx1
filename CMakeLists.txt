cmake_minimum_required(VERSION 3.0.0)
project(VsCodeCMakeTets VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()
# set(CMAKE_TEST_DEBUG "-std=c99 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_DEBUG "-std=c99 -pedantic-errors -Wall -Werror -DDG_TESTER")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${CMAKE_TEST_DEBUG})

add_executable(Q1Tester Dry/q1Tester.c Dry/q1.c)
add_executable(MapTester MtmEx1Tester/map_tests.c mtm_map/map.c MtmEx1Tester/utils.c)

#link_directories(.)
add_executable(ElectionTester
               MtmEx1Tester/election_tests.c
               election/election.c
               mtm_map/map.c
               election/augmented_map.c
               MtmEx1Tester/utils.c)
# target_link_libraries(ElectionTester libmap.a)

add_executable(Q2Tester Dry/q2.c)