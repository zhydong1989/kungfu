//
// Created by Keren Dong on 2019-06-20.
//

#ifndef WINGCHUN_TRADER_H
#define WINGCHUN_TRADER_H

#include <kungfu/yijinjing/log/setup.h>
#include <kungfu/yijinjing/io.h>
#include <kungfu/practice/apprentice.h>
#include <kungfu/wingchun/storage/storage.h>
#include <kungfu/wingchun/calendar/calendar.h>
#include <kungfu/wingchun/portfolio/account_manager.h>
#include <kungfu/wingchun/oms/def.h>

namespace kungfu
{
    namespace wingchun
    {
        namespace gateway
        {
            class Trader : public practice::apprentice
            {
            public:
                explicit Trader(bool low_latency, yijinjing::data::locator_ptr locator, const std::string &source, const std::string &account_id);

                virtual ~Trader() = default;

                const std::string &get_account_id() const
                { return account_id_; }

                const std::string &get_source() const
                { return source_; }

                Calendar& get_calendar()
                { return calendar_; }

                virtual const AccountType get_account_type() const = 0;

                virtual bool insert_order(const yijinjing::event_ptr& event) = 0;

                virtual bool cancel_order(const yijinjing::event_ptr& event) = 0;

                virtual bool req_position() = 0;

                virtual bool req_account() = 0;

                std::vector<uint64_t> get_pending_orders(const std::string &client_id = "") const;

            protected:
                void react(rx::observable<yijinjing::event_ptr> events) override ;

                void start() override ;

            private:
                void init_account_manager();

            private:
                std::string source_;
                std::string account_id_;

                storage::OrderStorage order_storage_;
                storage::TradeStorage trade_storage_;

                Calendar calendar_;

                AccountManager account_manager_;
//                oms::OrderManager order_manager_;

                std::vector<msg::data::Position> rsp_pos_;
                std::vector<msg::data::Position> rsp_pos_detail_;

            };
        }
    }
}

#endif //WINGCHUN_TRADER_H
