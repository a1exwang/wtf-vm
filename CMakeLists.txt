cmake_minimum_required(VERSION 3.5)
project(wtf_vm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        src/VirtualMachine.cpp
        src/MachineInst.cpp
        src/DataValue.cpp src/DataValue.h src/Binding.cpp src/Binding.h src/WtfException.cpp src/WtfException.h)
add_executable(wtf-vm ${SOURCE_FILES})