cmake_minimum_required(VERSION 3.8)
project(SOproject)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES cliente.c servidor.c)
add_executable(SOproject ${SOURCE_FILES} admin.c)
add_executable(servidor servidor.c)
add_executable(cliente cliente.c)
add_executable(admin admin.c)