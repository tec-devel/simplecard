/* 
 * File:   Table.cpp
 * Author: alexey
 * 
 * Created on 15 Май 2014 г., 0:42
 */

#include "Table.h"
#include "Player.h"

#include <algorithm>
#include <vector>
#include <string.h>
#include <iostream>

using namespace cardsrv;
using namespace std;

int Table::table_count = 0;

Table::Table(int players_count) :
max_players((players_count > MAX_PLAYERS) ? MAX_PLAYERS : players_count)
{
    m_id = table_count++;
    m_state = Table::WaitPlayersConnect;

    Slot *tmp_slot = 0;
    for (int i = 1; i < MAX_SLOTS + 1; i++)
    {
        tmp_slot = new Slot(MAX_CARDS_IN_SLOT, this);
        m_number_to_slot.insert(make_pair(i, tmp_slot));
        m_slots.push_back(tmp_slot);
    }

    init();
}

Table::~Table()
{
    std::map<int, Slot*>::iterator it;
    for (int i = 1; i < MAX_SLOTS + 1; i++)
    {
        if ((it = m_number_to_slot.find(i)) != m_number_to_slot.end())
            delete (*it).second;
    }
}

void Table::movePlayerPointer(std::list<Player*>::iterator& it)
{
    std::list<Player*>::iterator start_it;
    if (*it == m_players.back())
        it = m_players.begin();
    else
        ++it;

    while ((*it)->cards().empty())
    {
        if (*it == m_players.back())
            it = m_players.begin();
        else
            ++it;

        /// возможно прошлись по кругу
        if (it == start_it)
            break;
    }
}

void Table::actualizePlayers()
{

    for (list<Player*>::iterator it = m_players.begin(); it != m_players.end(); ++it)
        (*it)->setActivity(Player::Enabled);

    list <Player*>::iterator tmp_it;

    if (m_active_player)
    {
        m_active_player->setStatus(Player::Neutral);
        movePlayerPointer(act_player_it);
    }

    m_active_player = *act_player_it;
    m_active_player->setStatus(Player::Active);

    tmp_it = act_player_it;
    movePlayerPointer(tmp_it);

    m_passive_player = *tmp_it;
    m_passive_player->setStatus(Player::Passive);

    if (m_active_player == m_passive_player)
    {
        cout << "finish nahui" << endl;
    }
}

bool Table::isFinishedDeal()
{
    bool ret_code = m_flushed_player == m_players.size() - 1 && m_active_slots == 0;
    if (ret_code)
    {
        map<int, Slot*>::iterator it;
        for (int i = 1; i < MAX_SLOTS + 1; i++)
        {
            it = m_number_to_slot.find(i);
            if ((*it).second->status() != Slot::Empty)
                (*it).second->clear();
        }
    }

    return ret_code;
}

void Table::init()
{

    m_active_player = NULL;
    m_passive_player = NULL;

    m_active_slots = 0;

    m_flushed_player = 0;

    memset(m_is_on_table, 0, sizeof (m_is_on_table));

    for (list<Player*>::iterator it = m_players.begin(); it != m_players.end(); ++it)
        (*it)->setActivity(Player::Enabled);
}

void Table::next()
{
    m_state = WaitFirstTurn;

    actualizePlayers();
}

void Table::fillDeck()
{
    //    for (int i = 0; i < Card::Diamonds; i++)
    //        for (int j = 0; j < Card::Six; j++)
    //            m_deck_list.push_back(Card((Card::Suit)i, (Card::Value)j));
    for (int i = 0; i < Card::Hearts + 1; i++)
        for (int j = 0; j < Card::Six + 1; j++)
            m_deck_list.push_back(Card((Card::Suit)i, (Card::Value)j));

    random_shuffle(m_deck_list.begin(), m_deck_list.end());
}

void Table::prepare()
{
    Card::Value tmp_card = Card::Ace, test_card;

    bool have_trump = false;
    std::list<Player*>::iterator it = m_players.begin();
    if ((*it)->getJuniorTrump(tmp_card))
    {
        have_trump = true;
        act_player_it = it;
    }

    ++it;
    for (; it != m_players.end(); ++it)
    {
        if ((*it)->getJuniorTrump(test_card) && test_card > tmp_card)
        {
            act_player_it = it;
            have_trump = true;
            tmp_card = test_card;
        }
    }

    if (!have_trump)
        act_player_it = m_players.begin();

    actualizePlayers();
}

void Table::go()
{
    init(); // обнуляем указатели 
    fillDeck(); // заполняем массив колоды
    deal(); // сдача карт
    prepare(); // подготовка игры (у кого младший козырь)
    start(); // поехали!
}

void Table::start()
{
}

bool Table::deal()
{
    list<Player*>::iterator it;
    list<Card> tmp_list;

    bool is_deck_empty = false; // возвращает остались ли в колоде карты

    vector<Card>::iterator card_it = m_deck_list.begin();
    card_it += m_players.size() * FIRST_DEAL_SIZE;
    m_trump = (*card_it).suit();

    //    Card tmp_card = *card_it;
    //    m_deck_list.erase(card_it);
    //    m_deck_list.push_back()

    for (it = m_players.begin(); it != m_players.end(); ++it)
    {
        for (int j = 0; j < ((int) FIRST_DEAL_SIZE - (int) (*it)->cards().size()) && !is_deck_empty; j++)
        {
            if (!m_deck_list.empty())
            {
                tmp_list.push_back(m_deck_list.back());
                m_deck_list.pop_back();
            }
            else
            {
                is_deck_empty = true;
            }
        }

        if (!tmp_list.empty())
        {
            (*it)->addCardFromDeal(tmp_list);
            tmp_list.clear();
        }
        else
        {
            if ((*it)->cards().empty())
                (*it)->setActivity(Player::Disabled);
        }
    }

    return m_deck_list.empty();
}

void Table::playerTakeCard(Player* player)
{
    map<int, Slot*>::iterator it;
    list<Card> tmp_list;
    for (int i = 1; i < MAX_SLOTS + 1; i++)
    {
        it = m_number_to_slot.find(i);
        if ((*it).second->status() != Slot::Empty)
        {
            vector<Card> test_list = (*it).second->cards();
            for (vector<Card>::iterator card_it = test_list.begin();
                    card_it != test_list.end();
                    ++card_it)
                tmp_list.push_back(*card_it);

            if (!tmp_list.empty())
                player->addCardFromTable(tmp_list);

            (*it).second->clear();
        }
    }

    m_active_slots = 0; // все, активных слотов нет - потому что они пустые
    m_state = PassiveTakeCards;
}

void Table::flush(Player* player)
{
    if (player->activity() != Player::Disabled)
        if (player != m_passive_player)
        {
            player->setActivity(Player::Disabled);
            m_flushed_player++;

            if (isFinishedDeal())
            {
                // next();
                if (m_state == PassiveTakeCards)
                    actualizePlayers();

                m_active_slots = 0;
                m_state = WaitFirstTurn;
                m_flushed_player = 0;
                deal();
                actualizePlayers();
                memset(m_is_on_table, 0, sizeof (m_is_on_table));

            }
        }
}

Player* Table::addPlayer(int id)
{
    Player *player = 0;

    if (m_players.size() < max_players)
    {
        player = new Player(this, id);
        m_players.push_back(player);
        m_id_to_players.insert(make_pair(player->localId(), player));
    }

    if (m_players.size() == max_players)
    {
        if (m_state == WaitPlayersConnect)
        {
            m_state = WaitFirstTurn;
            go();
        }
    }

    return player;
}

bool Table::putCard(Player* player, Card card, int slot_number)
{
    if (player->activity() == Player::Disabled)
        return false;

    if (m_state != WaitFirstTurn && !m_is_on_table[card.value()])
        if (player != m_passive_player)
            return false;

    if (slot_number > MAX_SLOTS)
        return false;

    switch (m_state)
    {
    case WaitPlayersTurn:
    case PassiveTakeCards:
        break;
    case WaitFirstTurn:
    case WaitPlayerTurn:
        if (player != m_active_player)
            return false;

        m_state = WaitPlayersTurn;
        break;
    case WaitPlayersConnect:
    default:
        return false;
    }

    bool ret_code = true;

    std::map<int, Slot*>::iterator it;

    if ((it = m_number_to_slot.find(slot_number)) != m_number_to_slot.end())
    {
        Slot::SlotStatus tmp_status = (*it).second->status();
        if (tmp_status != Slot::Ok) // если слот не готов (карта не побита)
        {
            if (tmp_status == Slot::Empty) // если слот пустой
            {
                if (player != m_passive_player) // тогда карту положить на него может только НЕ пассивный игрок
                {
                    (*it).second->putCard(card);
                    m_active_slots++;
                }
                else
                    ret_code = false;
            }
            else if (tmp_status == Slot::Wait) // если слот в состоянии оржиания (то есть с ОДНОЙ картой)
            {
                if (player == m_passive_player) // тогда карту на него может положить ТОЛЬКО пассивный игрок
                {
                    if (Slot::Ok != (*it).second->putCard(card))
                        ret_code = false;
                    else
                        m_active_slots--;

                    if (isFinishedDeal())
                    {
                        // next();
                        m_active_slots = 0;
                        m_state = WaitFirstTurn;
                        m_flushed_player = 0;
                        deal();
                        actualizePlayers();
                        memset(m_is_on_table, 0, sizeof (m_is_on_table));

                    }
                }
                else
                    ret_code = false;
            }
        }
        else
            ret_code = false;
    }
    else
        ret_code = false;

    if (ret_code)
    {
        m_is_on_table[card.value()] = true;
    }

    return ret_code;
}

string Table::toString()
{
    string ret_str;
    map<int, Slot*>::iterator it;
    for (int i = 1; i < MAX_SLOTS + 1; i++)
    {
        it = m_number_to_slot.find(i);

        ret_str += (*it).second->toString();
        ret_str += " ";
    }

    return ret_str;
}
