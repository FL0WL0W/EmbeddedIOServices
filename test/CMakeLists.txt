# CMakeList.txt : CMake project for EmbeddedIOServicesUnitTests, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.30[3.30])

set(TARGET		EmbeddedIOServicesUnitTests)
set(HDR_DIR		include)
set(SRCS_DIR	src)

project(${TARGET} CXX)

# headers are always in include/
file(GLOB_RECURSE HDRS CONFIGURE_DEPENDS ${HDR_DIR}/*.h)
include_directories(${HDR_DIR})
# sources are always in src/
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ${SRCS_DIR}/*.cpp)

# create executable
add_executable(${TARGET} ${SRCS})

# link static libraries
target_link_libraries(${TARGET} EmbeddedIOServices)
target_link_libraries(${TARGET} EmbeddedIOServicesMockedObjects)
Set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY    https://github.com/google/googletest
    GIT_TAG           main
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gtest)
target_link_libraries(${TARGET} gtest gtest_main gmock)

add_test(ALLTESTS ${TARGET})