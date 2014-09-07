cmake_minimum_required(VERSION 2.8)

project (simplecards)

set (SOURCES
	Card.cpp
	Player.cpp
	Slot.cpp
	Table.cpp)

set (HEADERS
	Card.h
	Player.h
	Slot.h
	Table.h)

add_library (simplecards SHARED ${SOURCES})

install (TARGETS simplecards DESTINATION /usr/local/lib)
install (FILES ${HEADERS} DESTINATION /usr/local/include/libsimplecards)

