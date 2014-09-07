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

