cmake_minimum_required(VERSION 3.12)
project(point_cbase)

set(CMAKE_C_STANDARD 90)

add_library(point-cbase
        src/atom.h
        src/atom.c
        src/scopeguard.h
        src/scopeguard.c
        src/ptrbag.h
        src/ptrbag.c
        src/blocklist.h
        src/blocklist.c
        src/pool.h
        src/pool.c
        src/stopwatch.h
        src/stopwatch.c
        src/fmt.h
        src/fmt.c
        src/str.h
        src/str.c
        src/stringview.h
        src/stringview.c
        src/array.h
        src/array.c
        src/list.h
        src/list.c
        src/table.h
        src/table.c
        src/set.h
        src/set.c
        src/bit.h
        src/bit.c
        src/ring.h
        src/ring.c
        src/stack.c src/stack.h src/bitree.c src/bitree.h)

add_subdirectory(examples)

add_executable(scopeguard.t src/scopeguard.t.c)
target_link_libraries(scopeguard.t point-cbase)

add_executable(ptrbag.t src/ptrbag.t.c)
target_link_libraries(ptrbag.t point-cbase)

add_executable(blocklist.t src/blocklist.t.c)
target_link_libraries(blocklist.t point-cbase)

add_executable(pool.t src/pool.t.c)
target_link_libraries(pool.t point-cbase)

add_executable(fmt.t src/fmt.t.c)
target_link_libraries(fmt.t point-cbase)

add_executable(str.t src/str.t.c)
target_link_libraries(str.t point-cbase)

add_executable(stringview.t src/stringview.t.c)
target_link_libraries(stringview.t point-cbase)

add_executable(array.t src/array.t.c)
target_link_libraries(array.t point-cbase)

add_executable(list.t src/list.t.c)
target_link_libraries(list.t point-cbase)

add_executable(atom.t src/atom.t.c)
target_link_libraries(atom.t point-cbase)

add_executable(set.t src/set.t.c)
target_link_libraries(set.t point-cbase)

add_executable(bit.t src/bit.t.c)
target_link_libraries(bit.t point-cbase)

add_executable(ring.t src/ring.t.c)
target_link_libraries(ring.t point-cbase)
