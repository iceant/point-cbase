cmake_minimum_required(VERSION 3.0)
project(point-cbase-examples)

include_directories(../src)

add_executable(wf wf.c getword.c)
target_link_libraries(wf point-cbase)

add_executable(xref xref.c getword.c)
target_link_libraries(xref point-cbase)

add_executable(domain.t domain.t.c domain.c)
target_link_libraries(domain.t point-cbase)

add_executable(malloc.t malloc.t.c)
