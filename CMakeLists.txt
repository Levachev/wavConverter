cmake_minimum_required(VERSION 3.16.3)
project(audio)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(audio
        IConverter.h
        MixConverter.cpp
        MixConverter.h
        InsertConverter.cpp
        InsertConverter.h
        MuteConverter.cpp
        MuteConverter.h
        Instructions.cpp
        Instructions.h
        main.cpp
        Factory.cpp
        Factory.h
        Process.cpp
        Process.h
        func.cpp
        func.h
        StructOfInstructions.h
        )
