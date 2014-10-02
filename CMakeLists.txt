cmake_minimum_required(VERSION 2.8)

project (simplecards)

set(CMAKE_CXX_FLAGS "-std=c++0x")

set (SOURCES
        AbstractPlayer.cpp
        AbstractTable.cpp
        GameEvent.cpp
        EventlyPlayer.cpp
	Card.cpp
	Player.cpp
	Slot.cpp
	Table.cpp)

set (HEADERS
        AbstractPlayer.h
        AbstractTable.h
        EventlyPlayer.h
        GameEvent.h
	Card.h
	Player.h
	Slot.h
	Table.h)

add_library (simplecards SHARED ${SOURCES})

install (TARGETS simplecards DESTINATION /usr/local/lib)
install (FILES ${HEADERS} DESTINATION /usr/local/include/libsimplecards)

