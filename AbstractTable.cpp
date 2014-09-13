/* 
 * File:   AbstractTable.cpp
 * Author: alexey
 * 
 * Created on 13 Сентябрь 2014 г., 14:06
 */

#include "AbstractTable.h"

using namespace cardsrv;

int AbstractTable::table_count = 0;

AbstractTable::AbstractTable(int players_count)
: max_players(players_count)
{
    m_id = table_count++;
    m_table_state = Waiting;
}


AbstractTable::~AbstractTable()
{
}

void AbstractTable::startGame()
{
    m_table_state = Playing;
    start();
}

AbstractPlayer* AbstractTable::addPlayer(int gid)
{
    AbstractPlayer *ret_player = 0;

    if (m_players.size() < max_players)
    {
        ret_player = player(gid); // new Player(this, id);

        if (ret_player)
            m_players.push_back(ret_player);
        //m_id_to_players.insert(make_pair(player->localId(), player));
    }

    return ret_player;
}