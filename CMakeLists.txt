cmake_minimum_required(VERSION 2.8) 
project(can_test) 

add_compile_options(-std=c++11)
include_directories(include)
aux_source_directory(src CURRENT_SOURCES)
add_executable(can_test ${CURRENT_SOURCES}) 




