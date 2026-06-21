cmake_minimum_required(VERSION 3.14)
project(trino-cpp-example
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Dependencies ──────────────────────────────────────────────────────────────

find_package(CURL REQUIRED)

# ── Library: trino-query ──────────────────────────────────────────────────────

add_library(trino-query STATIC
    src/trino-query.cpp
)

target_include_directories(trino-query
    PUBLIC  include          # consumers get <trino-query.h>
)

target_link_libraries(trino-query
    PUBLIC  CURL::libcurl
)

# ── Executable: trino-example ─────────────────────────────────────────────────

add_executable(trino-example
    src/main.cpp
)

target_link_libraries(trino-example
    PRIVATE trino-query
)
