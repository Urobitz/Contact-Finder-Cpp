cmake_minimum_required(VERSION 3.10)
project(ContactFinder)

set(CMAKE_CXX_STANDARD 17)

add_executable(ContactFinder src/main.cpp src/sql_functions.cpp)
target_include_directories(ContactFinder PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(SQLite3 REQUIRED)
if(SQLite3_FOUND)
    target_link_libraries(ContactFinder PRIVATE ${SQLite3_LIBRARIES})
endif()
