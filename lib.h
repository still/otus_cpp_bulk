#pragma once

#include "version.h"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <ctime>

int version();

namespace otus {

    using Command   = std::string;
    using Commands  = std::list<Command>;
    using BulkSlot  = std::function<void(std::list<std::string>)>;
    using BulkSlots = std::list<BulkSlot>;
    using TimeSlot  = std::function<void(time_t)>;
    using TimeSlots = std::list<TimeSlot>;

    class Bulk {
    public:
        Bulk(int cpb) : m_cpb(cpb), m_brackets(0) {}
        void append(const std::string& str) {
            if(str == "{") {
                ++m_brackets;
                if(m_commands.size() > 0)
                    notifyBulk();
                return;
            }

            if(str == "}") {
                if(m_brackets > 0 && --m_brackets == 0)
                    notifyBulk();
                return;
            }

            m_commands.push_back(str);
            if(m_commands.size() == 1)
                notifyTime();

            if(m_brackets == 0 && m_commands.size() == m_cpb)
                notifyBulk();
        }

        void subscribeBulk(const BulkSlot& slot) {
            m_bulkSlots.push_back(slot);
        }

        void subscribeTime(const TimeSlot& slot) {
            m_timeSlots.push_back(slot);
        }
    private:
        void notifyBulk() {
            if(m_commands.size() > 0) {
                for(const auto& slot: m_bulkSlots)
                    slot(m_commands);
                m_commands.clear();
            }
        }
        void notifyTime() {
            time_t time = std::time(0);
            for(const auto& slot: m_timeSlots)
                slot(time);
        }
    private:
        size_t      m_cpb;
        Commands    m_commands;
        BulkSlots   m_bulkSlots;
        TimeSlots   m_timeSlots;
        size_t      m_brackets;
    };

    class BulkProcessor {
    public:
        BulkProcessor() {}
        void process(const Commands& commands) {
            std::cout << "bulk:";
            for (auto command: commands)
                std::cout << " " << command;
            std::cout << std::endl;
        }
    };

    class BulkLogger {
    public:
        BulkLogger() {}
        void dump(const Commands& commands) {
            std::ofstream file;
            file.open("bulk" + std::to_string(m_time) + ".log");
            file << "bulk:";
            for (auto command: commands)
                file << " " << command;
            file << std::endl;
            file.close();
        }
        void time(time_t time) {
            m_time = time;
            std::cout << time << std::endl;
        }
    private:
        time_t  m_time;
    };
}
