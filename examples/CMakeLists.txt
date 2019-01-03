cmake_minimum_required(VERSION 3.0)
project(point-cbase-examples)

include_directories(../src)

add_executable(wf wf.c getword.c)
target_link_libraries(wf point-cbase)

add_executable(xref xref.c getword.c)
target_link_libraries(xref point-cbase)