/* 
 * File:   AbstractPlayer.h
 * Author: alexey
 *
 * Created on 13 Сентябрь 2014 г., 13:39
 */

#ifndef ABSTRACTPLAYER_H
#define	ABSTRACTPLAYER_H

#include <string>
#include <vector>


namespace cardsrv {

    class AbstractTable;

    class AbstractPlayer {
        int m_global_id;

        std::string m_get_token;
        std::string m_put_token;
        
        AbstractTable *m_table;

        AbstractPlayer(const AbstractPlayer& orig);
    protected:
        AbstractPlayer(AbstractTable *table, const int& gid);

        AbstractTable *table()
        {
            return m_table;
        }

        AbstractTable *const_table() const
        {
            return m_table;
        }
        
    public:
       
        virtual ~AbstractPlayer();

        int id() const {
            return m_global_id;
        }

        void setGetToken(const std::string& token) {
            m_get_token = token;
        }

        std::string tokenGet() const {
            return m_get_token;
        }

        void setPutToken(const std::string& token) {
            m_put_token = token;
        }

        std::string tokenPut() const {
            return m_put_token;
        }

        virtual void get(const std::vector<std::string>& restful_data, std::string& responce) const = 0;
        virtual void put(const std::vector<std::string>& restful_data, const std::string& request, std::string& responce) = 0;
        
    };

}

#endif	/* ABSTRACTPLAYER_H */

