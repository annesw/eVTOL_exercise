project(eVTOL)
cmake_minimum_required(VERSION 3.0)

add_executable(eVTOL chargers_and_queue.cpp simulation_manager.cpp aircraft.cpp main.cpp)
