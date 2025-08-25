cmake_minimum_required(VERSION 3.15)
project(GraphOperator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(GraphOperator graph_operator.cpp)