cmake_minimum_required(VERSION 3.26)
project(TrabalhoFinalED C)

set(CMAKE_C_STANDARD 11)

add_executable(TrabalhoFinalED main.c
        commandQueue.c
        commandQueue.h
        petType.h
        petType.c
        pet.h
        person.h
        pet.c
        person.c
        program.h
)
