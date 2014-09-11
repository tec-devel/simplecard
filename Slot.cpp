/* 
 * File:   Slot.cpp
 * Author: alexey
 * 
 * Created on 15 Май 2014 г., 2:07
 */

#include <stdlib.h>

#include "Slot.h"
#include "Table.h"

using namespace cardsrv;
using namespace std;

Slot::Slot(int max_cards, Table* table)
{
    m_status = Slot::Empty;
    m_max_cards = max_cards;
    m_table = table;
}

Slot::Slot(const Slot& orig)
{
}

Slot::~Slot()
{
}

string Slot::toString()
{
    string ret_str = "[ ";
    for(vector<Card>::iterator it = m_cards.begin(); it != m_cards.end(); ++it)
    {
        ret_str += (*it).toString();
    ret_str+= " ";
    }
    
    ret_str+= "]";
    
    return ret_str;
}

Slot::SlotStatus Slot::putCard(Card card)
{
    if (m_cards.empty())
    {
        m_cards.push_back(card);
        m_status = Wait;
    }
    else if (m_cards.size() < m_max_cards)
    {
        Card tmp_card = m_cards.front();
        if (card.suit() == tmp_card.suit()) // если масти равны - то просто сравниваем значения карт
        {
            if (card.value() < tmp_card.value())
            {
                m_cards.push_back(card);
                m_status = Ok;
            }
        }
        else if (card.suit() == m_table->trump()) // если масти не равны - то надо проверить, может пришел козырь
        {
                m_cards.push_back(card);
                m_status = Ok;
        }
        else
            m_status = Wait;
    }

    return m_status;
}

