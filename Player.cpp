/* 
 * File:   Player.cpp
 * Author: alexey
 * 
 * Created on 15 Май 2014 г., 0:57
 */

#include <map>

#include "Player.h"
#include "Card.h"
#include "Table.h"

using namespace cardsrv;
using namespace std;

int Player::player_count = 0;

Player::Player(Table* table, int gid)
{
    m_global_id = gid;
    m_local_id = player_count++;
    m_table = table;
    have_junior_trump = false;
    m_junior_trump = Card::Ace;
    m_status = Neutral;
    m_activity = Enabled;
}

Player::Player(const Player& orig)
{
}

Player::~Player()
{
}

void Player::addCardFromTable(list<Card> cards)
{
    list<Card>::iterator it;
    for (it = cards.begin(); it != cards.end(); ++it)
        m_str_to_cards.insert(make_pair((*it).toString(), *it));
}

//void Player::addCardFromCurrentDeal(Card card)
//{
//    if(m_str_to_cards.size() < FIRST_DEAL_SIZE)
//    {
//        m_str_to_cards.insert(card.toString(), card);
//        return true;
//    }
//    else
//        return false;
//}

void Player::addCardFromDeal(list<Card> cards)
{
    list<Card>::iterator it;

    for (it = cards.begin(); it != cards.end(); ++it)
    {
        if ((*it).suit() == m_table->trump())
            if ((*it).value() > m_junior_trump)
            {
                have_junior_trump = true;
                m_junior_trump = (*it).value();
            }

        m_str_to_cards.insert(make_pair((*it).toString(), *it));
    }
}

string Player::genToken()
{
    m_last_token = string("1");
    return m_last_token;
}

Player::TurnStatus Player::turnFromClient(list<Card> cards, std::string token, int slot_number)
{
    if (m_activity == Disabled)
        return Fail;

    TurnStatus turn_status = Success;
    bool can_turn = true;
    bool is_put_on_table = true;
    list<Card> turn_list;

    if (m_last_token == token)
    {

        list<Card>::iterator it;

        for (it = cards.begin(); it != cards.end() && can_turn; ++it)
        {
            if (m_str_to_cards.find((*it).toString()) != m_str_to_cards.end())
                turn_list.push_back(*it);
            else
                can_turn = false;
        }

        if (can_turn)
        {

            for (it = turn_list.begin(); it != turn_list.end() && is_put_on_table; ++it)
            {
                is_put_on_table = m_table->putCard(this, *it, slot_number);
                if (is_put_on_table)
                    m_str_to_cards.erase((*it).toString());
            }

            if (m_str_to_cards.empty())
                m_table->flush(this);
        }
        else
            turn_status = Fail;
    }
    else
        turn_status = Fail;


    return turn_status;
}

void Player::flush()
{
    m_table->flush(this);
}

void Player::takeTableCards()
{
    if(m_status == Passive)
       m_table->playerTakeCard(this);
}

bool Player::getJuniorTrump(Card::Value& val)
{
    //    if (have_junior_trump)
    val = m_junior_trump;
    return have_junior_trump;
}

std::list<Player*> Player::table_players() const
{
    return m_table->players();
}

std::list<Slot*> Player::table_slots() const
{
    return m_table->slots();
}

std::string Player::toString() const
{
    map<string, Card>::const_iterator it = m_str_to_cards.begin();
    string ret_str;

    while (it != m_str_to_cards.end())
    {
        ret_str += (*it).second.toString();
        ret_str += " ";
        ++it;
    }

    return ret_str;
}

std::list<Card> Player::cards()
{
    map<string, Card>::iterator it = m_str_to_cards.begin();
    list<Card> ret_list;
    ret_list.clear();

    while (it != m_str_to_cards.end())
    {
        ret_list.push_back((*it).second);
        ++it;
    }

    return ret_list;
}


