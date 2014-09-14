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

#include <stdint.h>
#include <json/json_object.h>
#include <json/json.h>

#include <string.h>

#include <iostream>

using namespace cardsrv;
using namespace std;

int Player::player_count = 0;

Player::Player(Table* table, int gid)
: AbstractPlayer(table, gid)
{
    m_local_id = player_count++;
    have_junior_trump = false;
    m_junior_trump = Card::Ace;
    m_status = Neutral;
    m_activity = Enabled;
}

Player::~Player()
{
}

void Player::get(const std::vector<std::string>&, std::string& responce) const
{
    //    {
    //"players" : [
    //	{"id": 1, "cards": 5, "status": "passive", "activity": "enabled"},
    //	{"id": 2, "cards": 5, "status": "active", "activity": "enabled"},
    //	{"id": 3, "cards": 5, "status": "active", "activity": "enabled"}
    //	],
    //
    //"player": {
    //	"status": "active",
    //	"activity": "enabled",
    //	"cards":["C6","C7","C8","C9","C10","CJ"]
    //	},
    //
    //"table": {
    //	"trump": "CA",
    //	"slots": [
    //		{"number": 1, "down": "S6", "up": "S7"},
    //		{"number": 2, "down": "D7", "up": "D8"},
    //		{"number": 3, "down": "H8", "up": "H9"}
    //		]
    //	}
    //}

    json_object * jobj = json_object_new_object();

    json_object * jplayers_array = json_object_new_array();
    json_object * jtable_player = 0;
    json_object * jtable_player_id = 0;
    json_object * jtable_player_cards = 0;

    std::list<AbstractPlayer*> tmp_player_list = const_table()->const_players(); //player->table_players();

    for (std::list<AbstractPlayer*>::const_iterator it = tmp_player_list.begin();
            it != tmp_player_list.end();
            ++it)
    {
        Player* tmp_player = (Player*) (*it);

        json_object * jtable_player_status = 0;
        json_object * jtable_player_activity = 0;
        switch (tmp_player->status())
        {
        case Player::Active:
            jtable_player_status = json_object_new_string("active");
            break;
        case Player::Passive:
            jtable_player_status = json_object_new_string("passive");
            break;
        case Player::Neutral:
            jtable_player_status = json_object_new_string("neutral");
            break;
        }

        switch (tmp_player->activity())
        {
        case Player::Enabled:
            jtable_player_activity = json_object_new_string("enabled");
            break;
        case Player::Disabled:
            jtable_player_activity = json_object_new_string("disabled");
            break;
        }

        jtable_player = json_object_new_object();

        jtable_player_id = json_object_new_int(tmp_player->id());
        jtable_player_cards = json_object_new_int(tmp_player->cards().size());

        json_object_object_add(jtable_player, "id", jtable_player_id);
        json_object_object_add(jtable_player, "cards", jtable_player_cards);
        json_object_object_add(jtable_player, "status", jtable_player_status);
        json_object_object_add(jtable_player, "activity", jtable_player_activity);

        json_object_array_add(jplayers_array, jtable_player);
    }

    json_object_object_add(jobj, "players", jplayers_array);

    json_object * jplayer = json_object_new_object();
    json_object * jplayer_cards = json_object_new_array(); // json_object_new_string();
    json_object * jplayer_one_card = 0; // json_object_new_string();

    switch (m_status)
    {
    case Player::Active:
        json_object_object_add(jplayer, "status", json_object_new_string("active"));
        break;
    case Player::Passive:
        json_object_object_add(jplayer, "status", json_object_new_string("passive"));
        break;
    case Player::Neutral:
        json_object_object_add(jplayer, "status", json_object_new_string("neutral"));
        break;
    }

    switch (m_activity)
    {
    case Player::Enabled:
        json_object_object_add(jplayer, "activity", json_object_new_string("enabled"));
        break;
    case Player::Disabled:
        json_object_object_add(jplayer, "activity", json_object_new_string("disabled"));
        break;
    }

    std::list<Card> card_list = cards();
    for (std::list<Card>::const_iterator p_it = card_list.begin();
            p_it != card_list.end();
            ++p_it)
    {
        jplayer_one_card = json_object_new_string((*p_it).toString().data());
        json_object_array_add(jplayer_cards, jplayer_one_card);
    }

    json_object_object_add(jplayer, "cards", jplayer_cards);
    json_object_object_add(jobj, "player", jplayer);

    json_object * jtable = json_object_new_object();

    json_object * jtable_trump = json_object_new_int(((const Table*) const_table())->trump());
    json_object * jtable_slots = json_object_new_array();
    json_object * jtable_slot = 0;

    int i = 0;
    std::list<Slot*> tmp_slot_list = ((const Table*) const_table())->slots();
    for (std::list<Slot*>::const_iterator s_it = tmp_slot_list.begin();
            s_it != tmp_slot_list.end();
            ++s_it, i++)
    {
        jtable_slot = json_object_new_object();
        json_object_object_add(jtable_slot, "number", json_object_new_int(i));

        if ((*s_it)->cards().size() == 1)
        {
            json_object_object_add(jtable_slot, "down", json_object_new_string((*s_it)->cards()[0].toString().data()));
        }
        else if ((*s_it)->cards().size() == 2)
        {
            json_object_object_add(jtable_slot, "down", json_object_new_string((*s_it)->cards()[0].toString().data()));
            json_object_object_add(jtable_slot, "up", json_object_new_string((*s_it)->cards()[1].toString().data()));
        }

        json_object_array_add(jtable_slots, jtable_slot);
    }

    json_object_object_add(jtable, "trump", jtable_trump);
    json_object_object_add(jtable, "slots", jtable_slots);

    json_object_object_add(jobj, "table", jtable);

    responce.append(json_object_to_json_string(jobj));
}

void Player::put(const std::vector<std::string>&, const std::string& request, std::string& responce)
{
    //{
    //    "turn" : { "slot" : 1, "card" : "S6"}
    //}

    json_object * jobj = json_tokener_parse(request.data());

    int slot_number = -1;
    std::string card;
    std::string token;

    enum json_type type, turn_type;

    json_object_object_foreach(jobj, key, val)
    {
        type = json_object_get_type(val);
        switch (type)
        {
        case json_type_object:
            json_object_object_foreach(val, key1, val1)
        {
            turn_type = json_object_get_type(val1);
            switch (turn_type)
            {
            case json_type_int:
                slot_number = json_object_get_int(val1);
                break;
            case json_type_string:
                if (strcmp(key1, "card") == 0)
                    card.append(json_object_get_string(val1));
                break;
            }
        }
            break;
        }
    }

    json_object * ret_jobj = json_object_new_object();
    if (slot_number != -1 && !card.empty())
    {
        std::list<cardsrv::Card> turn_list;
        cardsrv::Card turn_card = cardsrv::Card::fromString(card);
        turn_list.push_back(turn_card);

        cardsrv::Player::TurnStatus status = turnFromClient(turn_list, slot_number);
        switch (status)
        {
        case cardsrv::Player::Success:
            json_object_object_add(ret_jobj, "status", json_object_new_string("success"));
            break;
        case cardsrv::Player::Fail:
            json_object_object_add(ret_jobj, "status", json_object_new_string("fail"));
            break;
        }
        responce.append(json_object_to_json_string(ret_jobj));
    }

    if (responce.empty())
    {
        json_object_object_add(ret_jobj, "status", json_object_new_string("fail"));
        responce.append(json_object_to_json_string(ret_jobj));
    }
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
        if ((*it).suit() == ((Table*) table())->trump())
            if ((*it).value() > m_junior_trump)
            {
                have_junior_trump = true;
                m_junior_trump = (*it).value();
            }

        m_str_to_cards.insert(make_pair((*it).toString(), *it));
    }
}

Player::TurnStatus Player::turnFromClient(list<Card> cards, int slot_number)
{
    if (m_activity == Disabled)
        return Fail;

    TurnStatus turn_status = Success;
    bool can_turn = true;
    bool is_put_on_table = true;
    list<Card> turn_list;

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
            is_put_on_table = ((Table*) table())->putCard(this, *it, slot_number);
            if (is_put_on_table)
                m_str_to_cards.erase((*it).toString());
        }

        if (m_str_to_cards.empty())
            ((Table*) table())->flush(this);
    }
    else
        turn_status = Fail;



    return turn_status;
}

void Player::flush()
{
    ((Table*) table())->flush(this);
}

void Player::takeTableCards()
{
    if (m_status == Passive)
        ((Table*) table())->playerTakeCard(this);
}

bool Player::getJuniorTrump(Card::Value& val)
{
    //    if (have_junior_trump)
    val = m_junior_trump;
    return have_junior_trump;
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

std::list<Card> Player::cards() const
{
    map<string, Card>::const_iterator it = m_str_to_cards.begin();
    list<Card> ret_list;
    ret_list.clear();

    while (it != m_str_to_cards.end())
    {
        ret_list.push_back((*it).second);
        ++it;
    }

    return ret_list;
}


