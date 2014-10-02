/* 
 * File:   EventlyPlayer.h
 * Author: alexey
 *
 * Created on 28 Сентябрь 2014 г., 18:21
 */

#ifndef EVENTLYPLAYER_H
#define	EVENTLYPLAYER_H

#include "AbstractPlayer.h"

#include <map>

#include <pthread.h>


namespace cardsrv {

    class EventlyPlayer : public AbstractPlayer {
        
        std::map <int, GameEventPtr> m_events;
        mutable pthread_mutex_t mutex;

    public:
        EventlyPlayer(AbstractTable *table, const int& gid);
//        EventlyPlayer(const EventlyPlayer& orig);
        virtual ~EventlyPlayer();

        void addEvent(GameEventPtr);
        
        virtual void get(const std::vector<std::string>& restful_data, std::string& responce) const;

    protected:
        std::map<int, GameEventPtr> events();
        void acceptEvent(int id);

    };
}

#endif	/* EVENTLYPLAYER_H */

