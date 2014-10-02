/* 
 * File:   EventlyPlayer.cpp
 * Author: alexey
 * 
 * Created on 28 Сентябрь 2014 г., 18:21
 */

#include "EventlyPlayer.h"

#include <json/json_object.h>
#include <json/json.h>

using namespace cardsrv;

EventlyPlayer::EventlyPlayer(AbstractTable *table, const int& gid)
: AbstractPlayer(table, gid)
{
    pthread_mutex_init(&mutex, 0);
}

//EventlyPlayer::EventlyPlayer(const EventlyPlayer& orig)
//{
//}

EventlyPlayer::~EventlyPlayer()
{
    pthread_mutex_destroy(&mutex);
}

std::map<int, GameEventPtr> EventlyPlayer::events()
{
    std::map<int, GameEventPtr> ret_map;

    pthread_mutex_lock(&mutex);
    ret_map = m_events;
    pthread_mutex_unlock(&mutex);

    return ret_map;
}

void EventlyPlayer::addEvent(GameEventPtr evt)
{
    pthread_mutex_lock(&mutex);
    m_events.insert(std::make_pair(evt->id(), evt));
    pthread_mutex_unlock(&mutex);
}

void EventlyPlayer::acceptEvent(int id)
{
    std::map<int, GameEventPtr>::iterator it;
    if (!m_events.empty() && (it = m_events.find(id)) != m_events.end())
    {
        (*it).second->accept();

        pthread_mutex_lock(&mutex);
        m_events.erase(it);
        pthread_mutex_unlock(&mutex);
    }
}

void EventlyPlayer::get(const std::vector<std::string>&, std::string& responce) const
{
    //   {
    //    "events" : { "id": "1", "data" : []}
    //   }

    std::map<int, GameEventPtr> ret_map;

    pthread_mutex_lock(&mutex);
    ret_map = m_events;
    pthread_mutex_unlock(&mutex);

    json_object * jobj = json_object_new_object();
    json_object * j_events = json_object_new_array();
    json_object * j_event = 0; // json_object_new_object();

    std::map<int, GameEventPtr>::iterator it;

    for (it = ret_map.begin(); it != ret_map.end(); ++it)
    {
        if (!(*it).second->isAccepted())
        {
            j_event = json_object_new_object();
            json_object_object_add(j_event, "id", json_object_new_int((*it).second->id()));
            json_object_object_add(j_event, "data", json_tokener_parse((*it).second->toJson().data()));
            json_object_array_add(j_events, j_event);
        }
    }
    
    json_object_object_add(jobj, "events", j_events);
    
    responce = json_object_to_json_string(jobj);
}
