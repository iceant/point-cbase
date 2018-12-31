cmake_minimum_required(VERSION 3.12)
project(point_cbase)

set(CMAKE_C_STANDARD 90)

add_library(point-cbase src/scopeguard.c src/scopeguard.t.c src/hexdump.c src/ptrbag.h src/ptrbag.c src/ptrbag.t.c src/blocklist.h src/blocklist.c src/blocklist.t.c src/pool.h src/pool.c src/pool.t.c src/stopwatch.h src/stopwatch.c)

add_executable(scopeguard.t src/scopeguard.t.c)
target_link_libraries(scopeguard.t point-cbase)

add_executable(ptrbag.t src/ptrbag.t.c)
target_link_libraries(ptrbag.t point-cbase)

add_executable(blocklist.t src/blocklist.t.c)
target_link_libraries(blocklist.t point-cbase)

add_executable(pool.t src/pool.t.c)
target_link_libraries(pool.t point-cbase)
