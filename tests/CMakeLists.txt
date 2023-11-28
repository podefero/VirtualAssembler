cmake_minimum_required(VERSION 3.14)
project(virtualmachine_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
#file(GLOB SOURCES "../src/*.cpp")
add_executable(tests vm_tests.cpp asm_tests.cpp ${SOURCES})

target_include_directories(tests PRIVATE ../include)

target_link_libraries( 
  tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(run_tests
    COMMAND tests
    DEPENDS tests
)

add_executable(integratedTest integrated_test.cpp ${SOURCES})

target_include_directories(integratedTest PRIVATE ../include)

target_link_libraries(
        integratedTest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(integratedTest)

add_custom_target(run_integrated_tests
        COMMAND integratedTest
        DEPENDS integratedTest
)
