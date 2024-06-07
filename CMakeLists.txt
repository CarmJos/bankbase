cmake_minimum_required(VERSION 3.28)
project(bankbase C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.archive)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.library)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.runtime)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/.runtime)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/.library)

add_executable(bankbase src/main.c
        src/account.c
        src/transaction.c
        src/transaction.h
        src/user.c
        src/user.h
        src/admin.c
        src/admin.h
        src/database.c
        src/database.h
        src/utils.c
        src/utils.h
)