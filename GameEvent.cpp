/* 
 * File:   GameEvent.cpp
 * Author: alexey
 * 
 * Created on 15 Сентябрь 2014 г., 3:05
 */

#include "GameEvent.h"

int GameEvent::event_counter = 0;

GameEvent::GameEvent()
: m_id(event_counter++)
{
}

GameEvent::~GameEvent()
{
}

