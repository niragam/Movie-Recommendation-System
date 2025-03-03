cmake_minimum_required(VERSION 3.10)

project(MovieRecommendations)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(src)

# Add library
add_library(movie_lib src/MovieManager.cpp src/user.cpp src/run.cpp)

# Add main executable
add_executable(movieApp src/main.cpp)
target_link_libraries(movieApp movie_lib)

# Add test executable
add_executable(testsMovies src/tests/testsMovies.cpp)
target_link_libraries(testsMovies gtest_main movie_lib)
add_test(NAME testsMovies COMMAND testsMovies)

# Install targets for easier container execution
install(TARGETS movieApp testsMovies RUNTIME DESTINATION /usr/src/project/build)