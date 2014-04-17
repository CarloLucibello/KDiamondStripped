cmake_minimum_required(VERSION 2.8)

set(kdiamond_SRCS
	main.cpp
	player.cpp
	gamestate.cpp
	gameparams.cpp
	game.cpp
	board.cpp
	diamond.cpp
)

find_package(Qt4 4.4.3 REQUIRED)
include(${QT_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable (kdiamond-stripped ${kdiamond_SRCS}) 
target_link_libraries(kdiamond-stripped ${QT_LIBRARIES})

