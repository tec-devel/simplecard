/* 
 * File:   Table.h
 * Author: alexey
 *
 * Created on 15 Май 2014 г., 0:42
 */

#ifndef TABLE_H
#define	TABLE_H

#include <list>
#include <vector>
#include <map>
#include "Slot.h"
#include "Player.h"
#include "AbstractTable.h"

#include <pthread.h>

#define MAX_PLAYERS 6
#define MAX_SLOTS 6
#define MAX_CARDS_IN_SLOT 2
#define FIRST_DEAL_SIZE 6

namespace cardsrv {

    class Player;

    class Table : public AbstractTable {
        friend class Player;
    public:

        enum State {
            PrepareGame,
            WaitFirstTurn,
            WaitPlayersConnect,
            WaitPlayerTurn,
            WaitPlayersTurn,
            PassiveTakeCards
        };

    private:

        mutable pthread_mutex_t table_mutex;

        Table::State m_state;

        std::map<int, Player*> m_id_to_players;
        // итератор для формирования указателей на активного и пассивного игрока

        std::list<AbstractPlayer*>::iterator act_player_it;

        int m_flushed_player;

        // номер слота на столе -> сам слот
        std::map<int, Slot*> m_number_to_slot;
        // список, чтобы было что отдавать клиентам
        std::list<Slot*> m_slots;

        int m_active_slots;

        // для хранения карт, котороые на даннцю сдачу на столе (чтобы подкидывать) 
        bool m_is_on_table[9];

        std::list<int> m_not_empty_slots;

        // собственно колода
        std::vector<Card> m_deck_list;

        Player* m_active_player;
        Player* m_passive_player;

        Card::Suit m_trump;


        void actualizePlayers();
        void movePlayerPointer(std::list<AbstractPlayer*>::iterator&);
        bool isFinishedDeal();
        void init();

        void next();

    public:
        Table(int players_count = 3);
        virtual ~Table();

        virtual AbstractPlayer* player(int gid) {
            return new Player(this, gid);
        }

        Card::Suit trump() const {
            return m_trump;
        }

        virtual void start();

        void fillDeck();
        void prepare();
        bool deal();

        void go();

        std::list<Slot*> slots() const {
            return m_slots;
        }

        // test
        std::string toString();

    protected:

        void flush_private(Player*);
        bool put_card_private(Player*, Card, int);
        void take_card_private(Player*);

        
        void flush(Player*);
        bool putCard(Player*, Card, int);
        void playerTakeCard(Player*);
    };

} // cardsrv

#endif	/* TABLE_H */

