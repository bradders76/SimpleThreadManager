
// *******************************************************
// ProcessItem.cpp
// *******************************************************
//
// Author: Bradley Crouch
// Copyright: © 2024 - April - 17
//
// Created by Bradley Crouch on 01/04/2024.
//


#ifndef SIMPLETHREADMANAGER_PROCESSITEM_HPP
#define SIMPLETHREADMANAGER_PROCESSITEM_HPP

#include <utility>

namespace SimpleThreadManager {
    class ProcessItem {
    protected:
        std::initializer_list<std::string> m_dependencies;
        std::string m_id;
        std::shared_ptr<IProcess> m_process;
        long m_priorityLevel;

    public:
        ProcessItem(std::string id, std::shared_ptr<IProcess> process, std::initializer_list<std::string> dependencies,
                    long priorityLevel)
                : m_id(std::move(id)), m_process(std::move(process)), m_dependencies(dependencies), m_priorityLevel(priorityLevel) {};

        std::string Id() { return m_id; }

        std::shared_ptr<IProcess> Process() { return m_process; }

        long PriorityLevel() const { return m_priorityLevel; }

        std::initializer_list<std::string> Dependencies() {
            return m_dependencies;
        }


        friend bool operator<(const ProcessItem &lhs, const ProcessItem &rhs);
    };


    bool operator<(const ProcessItem &lhs, const ProcessItem &rhs) {
        if (lhs.m_priorityLevel == rhs.m_priorityLevel) return lhs.m_id < rhs.m_id;

        return lhs.m_priorityLevel < rhs.m_priorityLevel;
    }
}

#endif
