/* 
 * File:   AbstractTable.h
 * Author: alexey
 *
 * Created on 13 Сентябрь 2014 г., 14:06
 */

#ifndef ABSTRACTTABLE_H
#define	ABSTRACTTABLE_H

#include <list>

namespace cardsrv {

    class AbstractPlayer;

    class AbstractTable {
    public:

        enum TableState {
            Waiting,
            Playing
        };

    private:
        static int table_count;

        const int max_players;
        int m_id;

        std::list<AbstractPlayer*> m_players;

        TableState m_table_state;

    public:
        AbstractTable(int players_count);
        virtual ~AbstractTable();

        int id() const {
            return m_id;
        }

        TableState state() const {
            return m_table_state;
        }

        virtual AbstractPlayer* addPlayer(int gid);
        virtual void removePlayer(int gid);

        bool isFull() {
            return m_players.size() == max_players;
        }
        
        const int maxPlayers() const
        {
            return max_players;
        }

        int realPlayers() const
        {
            return m_players.size();
        }

        void startGame();
        virtual void start() = 0;

        std::list<AbstractPlayer*>& players() {
            return m_players;
        }

        std::list<AbstractPlayer*> copy_players() {
            return m_players;
        }

        std::list<AbstractPlayer*> const_players() const {
            return m_players;
        }

    protected:
        virtual AbstractPlayer* player(int gid) = 0;


    };

}

#endif	/* ABSTRACTTABLE_H */

