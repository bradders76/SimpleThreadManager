// *******************************************************
// ProcessControl.cpp
// *******************************************************
//
// Author: Bradley Crouch
// Copyright: © 2024 - April - 17
//
// Created by Bradley Crouch on 01/04/2024.
//


#ifndef SIMPEQUEUE_SIMPLETHREADMANAGER_HPP
#define SIMPEQUEUE_SIMPLETHREADMANAGER_HPP


#include <map>
#include <list>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include <tuple>
#include <future>
#include <mutex>
#include <cmath>
#include <stdexcept>
#include <condition_variable>
#include <queue>
#include <algorithm>
#include <set>
#include <functional>

#include "../include/IProcess.hpp"
#include "../include/IData.hpp"
#include "../include/ProcessFunction.hpp"
#include "../include/ProcessItem.hpp"

using namespace std::chrono_literals;


namespace SimpleThreadManager {
    const ushort DEFAULT_THREADS = 8;
    const ushort DEFAULT_PRIORITY = 1000;


    class ProcessControl {
    private:
        std::map<std::string, ProcessItem> m_processes;

        std::mutex m_mutex;

        ushort m_maxThreads;  // not yet used

    public:

        ProcessControl(ushort maxThreads = DEFAULT_THREADS) {
            if (maxThreads == 0) maxThreads = 1;
            m_maxThreads = maxThreads;

        }

        void AddProcess(std::string id, std::shared_ptr<IProcess> process,
                        std::initializer_list<std::string> dependencies = {}, long priorityLevel = DEFAULT_PRIORITY);


        void AddProcess(std::string id, std::function<void(std::shared_ptr<IData>)> function,
                        std::initializer_list<std::string> dependencies = {}, long priorityLevel = DEFAULT_PRIORITY);

        bool CheckDependencies();

        void Run(std::shared_ptr<IData> data = nullptr);
    };
}
#endif //SIMPEQUEUE_SIMPLETHREADMANAGER_HPP
