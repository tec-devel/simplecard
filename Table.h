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

#define MAX_PLAYERS 6
#define MAX_SLOTS 6
#define MAX_CARDS_IN_SLOT 2
#define FIRST_DEAL_SIZE 6

namespace cardsrv
{
    
class Player;
    
class Table {
    friend class Player;
public:
    enum State
    {
        PrepareGame,
        WaitFirstTurn,
        WaitPlayersConnect,
        WaitPlayerTurn,
        WaitPlayersTurn,
        PassiveTakeCards
    };
    
private:
    const int max_players;
    
    Table::State m_state;
    
    std::map<int, Player*> m_id_to_players;
    // итератор для формирования указателей на активного и пассивного игрока
    
    std::list<Player*>::iterator act_player_it;
    
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
    
    std::list<Player*> m_players;
    Player* m_active_player;
    Player* m_passive_player;

    Card::Suit m_trump;
    
    int m_id;
    
    void actualizePlayers();
    void movePlayerPointer(std::list<Player*>::iterator&);
    bool isFinishedDeal();
    void init();
    
    void next();

public:
    static int table_count;
    
    Table(int players_count = 3);
    virtual ~Table();
    
    Player* addPlayer(int id);
    
    State state() const
    {
        return m_state;
    }
    
    Card::Suit trump()
    {
        return m_trump;
    }
    
    int id() const
    {
        return m_id;
    }
    
    void start();

    void fillDeck();
    void prepare();
    bool deal();
    void flush(Player*);
    
    void go();
    
    std::list<Slot*> slots() const
    {
        return m_slots;
    }
    
    std::list<Player*> players() const
    {
        return m_players;
    }
    
    // test
    std::string toString();
    
protected:
    
    bool putCard(Player*, Card, int);
    
    void playerTakeCard(Player*);

};

} // cardsrv

#endif	/* TABLE_H */

