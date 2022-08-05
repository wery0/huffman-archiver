cmake_minimum_required(VERSION 3.20)
project(HaffmanArchiver)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        HuffmanArchiver
        archiver.cpp
        Decoder.h
        Decoder.cpp
        Encoder.cpp
        Encoder.h
        BitReader.h
        BitReader.cpp
        BitWriter.h
        BitWriter.cpp
        HuffmanTree.h
        HuffmanTree.cpp
        PriorityQueue.h
        PriorityQueue.cpp)