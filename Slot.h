/* 
 * File:   Slot.h
 * Author: alexey
 *
 * Created on 15 Май 2014 г., 2:07
 */

#ifndef SLOT_H
#define	SLOT_H

#include <vector>
#include "Card.h"

namespace cardsrv
{
    
    class Table;

class Slot {
public:
    enum SlotStatus
    {
        Ok,
        Wait,
        Empty
    };
    
private:
    std::vector<Card> m_cards;
    int m_max_cards;
    
    SlotStatus m_status;
    Table *m_table;
    
    
public:
    Slot(int max_cards, Table* table);
    Slot(const Slot& orig);
    virtual ~Slot();
    
    void clear()
    {
        m_cards.clear();
        m_status = Empty;
    }
    
    SlotStatus status() const
    {
        return m_status;
    }
    
    std::vector<Card> cards() const
    {
        return m_cards;
    }
    
    SlotStatus putCard(Card);
    
    /// test
    std::string toString();
};

}

#endif	/* SLOT_H */

