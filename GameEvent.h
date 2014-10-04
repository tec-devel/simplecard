/* 
 * File:   GameEvent.h
 * Author: alexey
 *
 * Created on 15 Сентябрь 2014 г., 3:05
 */

#ifndef GAMEEVENT_H
#define	GAMEEVENT_H

#include <memory>
//#include <atomic>

class GameEvent {
    static int event_counter;
    int m_id;

//    std::atomic_bool m_accept_flag;
    bool m_accept_flag;

protected:
    GameEvent();
public:
    virtual ~GameEvent();

    int id() const {
        return m_id;
    }
    
    void accept()
    {
        m_accept_flag = true;
    }
    
    bool isAccepted() const
    {
        return m_accept_flag;
    }
    
    virtual std::string toJson() = 0;

};

typedef std::shared_ptr <GameEvent> GameEventPtr;

#endif	/* GAMEEVENT_H */

