cmake_minimum_required(VERSION 3.13)
project(Akina)

set(CMAKE_CXX_STANDARD 17)

add_executable(Akina main.cpp BasicStruct.hpp Command.hpp UserMananger.hpp FileManager.hpp TrainManager.hpp BPlusTree.hpp
        BasicString.hpp BasicStruct.hpp vector.hpp exceptions.hpp ExMemory.hpp)