/* 
 * File:   Card.h
 * Author: alexey
 *
 * Created on 15 Май 2014 г., 0:45
 */

#ifndef CARD_H
#define	CARD_H

#include <string>

namespace cardsrv
{

namespace cards_debug
{
}
    
class Card {
public:
    enum Suit {
        Spades = 0, // пики
        Clubs, // крести
        Diamonds, // бубны  
        Hearts // червы
    }; // масть

    enum Value {
        Ace = 0, // туз
        King, // король
        Queen, // дама
        Jack, // валет
        Ten, // десятка
        Nine, // девятка
        Eight, // восьмерка
        Seven, // семерка
        Six // шестерка
    };

private:
    Suit m_suit;
    Value m_value;
    
    static std::string suits[4];// = {"S", "C", "D", "H"};
    static std::string values[9];// = {"A", "K", "Q", "J", "10", "9", "8", "7", "6"};
    
public:
    Card(Card::Suit s, Card::Value v);
    Card(const Card& orig);
    
    std::string toString() const;
    Card fromJson(std::string);
    
    Suit suit() const
    {
     return m_suit;
    }
    
    Value value() const
    {
        return m_value;
    }
    
    virtual ~Card();
    
    
    Card& operator=(const Card& other);
    bool operator==(const Card& other);
};

}

#endif	/* CARD_H */

