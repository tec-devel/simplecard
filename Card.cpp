/* 
 * File:   Card.cpp
 * Author: alexey
 * 
 * Created on 15 Май 2014 г., 0:45
 */

#include "Card.h"

using namespace cardsrv;
using namespace std;

string Card::suits[4] = {"S", "C", "D", "H"};
string Card::values[9] = {"A", "K", "Q", "J", "10", "9", "8", "7", "6"};

Card::Card(Suit s, Value v)
{
m_suit = s;
m_value = v;
}

Card::Card(const Card& orig)
{
    m_suit = orig.m_suit;
    m_value = orig.m_value;
}

Card::~Card()
{
}

Card& Card::operator=(const Card& other)
{
    if(this == &other)
        return *this;
    
    m_suit = other.m_suit;
    m_value = other.m_value;
    
    return *this;
}

bool Card::operator==(const Card& other)
{
    if(this->m_suit == other.m_suit && this->m_value == other.m_value)
        return true;
    return false;
}

string Card::toString() const
{
    string tmp_str = suits[m_suit] + values[m_value];
    return tmp_str;
}

Card Card::fromString(std::string card)
{
    Card ret_card(Spades, Six);
    if(card[0] == 'S')
        ret_card.m_suit = Spades;
    else if(card[0] == 'C')
        ret_card.m_suit = Clubs;
    else if(card[0] == 'D')
        ret_card.m_suit = Diamonds;
    else if(card[0] == 'H')
        ret_card.m_suit = Hearts;
    
    if(card[1] == '6')
        ret_card.m_value = Six;
    else  if(card[1] == '7')
        ret_card.m_value = Seven;
    else  if(card[1] == '8')
        ret_card.m_value = Eight;
    else  if(card[1] == '9')
        ret_card.m_value = Nine;
    else  if(card[1] == '10')
        ret_card.m_value = Ten;
    else  if(card[1] == 'J')
        ret_card.m_value = Jack;
    else  if(card[1] == 'Q')
        ret_card.m_value = Queen;
    else  if(card[1] == 'K')
        ret_card.m_value = King;
    else  if(card[1] == 'A')
        ret_card.m_value = Ace;
    
    return ret_card;
}
