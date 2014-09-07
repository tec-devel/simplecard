/* 
 * File:   Player.h
 * Author: alexey
 *
 * Created on 15 Май 2014 г., 0:57
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <list>
#include "Card.h"
#include "Slot.h"

namespace cardsrv {

    class Table;
    class Slot;

    class Player {
        friend class Table;

    public:

        enum TurnStatus {
            /// ход завершен успешно; карта легла на стол, куда ее собирался положить игрок
            Success,
            /// ход завершен неудачно
            /// карта не легла на стол, куда ее собирался положить игрок;
            /// это может произойти по нескольким причинам - слот уже занят, игрок сейчас не может 
            /// походить на этот слот etc
            Fail
        };

        enum PlayerStatus {
            Active,
            Passive,
            Neutral,
            Waiting
        };

        enum PlayerActivity {
            Enabled,
            Disabled
        };

    private:
        static int player_count;

        Table* m_table;

        int m_global_id;
        int m_local_id;

        bool have_junior_trump;

        std::map<std::string, Card> m_str_to_cards;

        std::string m_last_token;

        // fool theme
        Card::Value m_junior_trump;

        PlayerStatus m_status;

        PlayerActivity m_activity;


        /**
         * Конструктор класса
         * Метод закрыт, должен создавать столом, если игрок может к нему присоединиться
         * @param table указатель на стол, за которым будет играть игрок
         * @param gid глобальный уникальный идентификатор игрока (пользователя)
         */
        Player(Table* table, int gid);
        /**
         * Конструктор копирования
         * тоже закрыт :)
         * @param orig
         */
        Player(const Player& orig);
    public:
        virtual ~Player();

        int localId() const {
            return m_local_id;
        }

        /**
         * Основной метод игрока для осуществления хода
         * @param cards список карт, которые игрок собирается положить на стол
         * @param token уникальный токен авторизации, который высылается игроку для
         * осуществления следущего хода (если токен не совпадает, ход отменяется, а
         * игроку высылается новый) 
         * @param slot_number номер слота, на который игрок может полжить карту 
         * (одну карту? а почему тогда список?)
         * @return статус хода (см. TurnStatus)
         */
        TurnStatus turnFromClient(std::list<Card> cards, std::string token, int slot_number);
        
        /**
         * Метод, означающий отбой
         */
        void flush();
        
        /**
         * Метод, генерирующий уникальный токен авторизации для следующего хода
         * если при осуществлении следующего хода этот токен не совпадает, ход
         * не совершается
         * @return уникальный токен авторизации
         */
        std::string genToken();

        /**
         * Игрок забирает карты со стола на руки
         */
        void takeTableCards();

        /**
         * Метод, возвращающий активность игрока
         * активность определяется правилами игры и текущим состоянием стола
         * @return активность
         */
        PlayerActivity activity() const {
            return m_activity;
        }

        /**
         * Метод, возвращающий статус игрока за игрой
         * статус определяется правилами игры (например для дурака - это игрок,
         * который ходит, игрок, под которого ходят и все остальные :) )
         * @return 
         */
        PlayerStatus status() const {
            return m_status;
        }
        
        
        /// методы для получения информации о столе и других игроках
        
        std::list<Player*> table_players() const;
        std::list<Slot*> table_slots() const;
        
        
        

        // test!
        std::string toString() const;
        std::list<Card> cards();

    private:
        void ready();

        // fool theme
        bool getJuniorTrump(Card::Value&);

        void setStatus(PlayerStatus status) {
            m_status = status;
        }

        void setActivity(PlayerActivity activity) {
            m_activity = activity;
        }



        void addCardFromTable(std::list<Card>);
        void addCardFromDeal(std::list<Card>);


    };

}

#endif	/* PLAYER_H */

