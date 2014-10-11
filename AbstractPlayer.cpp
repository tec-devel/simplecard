/* 
 * File:   AbstractPlayer.cpp
 * Author: alexey
 * 
 * Created on 13 Сентябрь 2014 г., 13:39
 */

#include <map>

#include "AbstractPlayer.h"
#include "GameEvent.h"

#include "AbstractTable.h"

using namespace cardsrv;

AbstractPlayer::AbstractPlayer(AbstractTable *table, const int& gid)
{
    m_table = table;
    m_global_id = gid;

}

AbstractPlayer::AbstractPlayer(const AbstractPlayer& orig)
{
}

AbstractPlayer::~AbstractPlayer()
{
    m_table->removePlayer(m_global_id);
}



