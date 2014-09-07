/* 
 * File:   main.cpp
 * Author: alexey
 *
 * Created on 15 Май 2014 г., 0:42
 */

#include <iostream>
#include "Table.h"
#include "Player.h"

int main(int, char**)
{
    cardsrv::Table tbl;
    cardsrv::Player* player1, *player2, *player3, *player4;
    
    player1 = tbl.addPlayer(1);
    player2 = tbl.addPlayer(2);
    player3 = tbl.addPlayer(3);
    player4 = tbl.addPlayer(4);

    tbl.go();
    
    std::cout << "end";
    
//    std::list<cardsrv::Card> cards;
//    cards.push_back(cardsrv::Card(cardsrv::Card::Spades, cardsrv::Card::Six));
//    player->genToken();
//    player->addCardFromDeal(cards, cardsrv::Card::Spades);
//    cardsrv::Player::TurnStatus status = player->turnFromClient(cards, "1", 1);
//    std::cout << status;
//    player->addCardFromDeal(cards, cardsrv::Card::Spades);
//    status = player->turnFromClient(cards, "1", 1);
//    std::cout << status;
    
    return 0;
}

