//
// Created by Bradley Crouch on 15/04/2024.
//



#include "../include/ProcessControl.hpp"


void ProcessControl::AddProcess(std::string id, std::shared_ptr<IProcess> process,
        std::initializer_list<std::string> dependencies, long priorityLevel)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(m_processes.find(id) != m_processes.end())
    {
        throw std::runtime_error("Error duplicate id ("  + id + ")");
    }
    m_processes.emplace(id,ProcessItem(id, process, dependencies, priorityLevel));

}

void ProcessControl::AddProcess(std::string id, std::function<void(std::shared_ptr<IData>)> function,
                std::initializer_list<std::string> dependencies, long priorityLevel)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if(m_processes.find(id) != m_processes.end())
    {
        throw std::runtime_error("Error duplicate id ("  + id + ")");
    }

    std::shared_ptr<IProcess> process = std::make_shared<ProcessFunction>(id, function);

    m_processes.emplace(id,ProcessItem(id, process, dependencies, priorityLevel));
}

bool ProcessControl::CheckDependiences()
{
    std::map<std::string, ProcessItem> processMap;
    std::set<std::string> completedSet;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        processMap = m_processes;
    }

    int finished = 0;

    do
    {
        finished = 0;

        if(processMap.empty()) break;

        std::list<std::string> remove;

        for(auto &item:processMap)
        {
            std::string id      = item.second.Id();
            auto dependancies   = item.second.Dependancies();
            bool completed      = true;

            for(auto item:dependancies)
            {
                if(!completedSet.contains(item))
                {
                    completed = false;
                    break;
                }
            }

            if(completed)
            {
                completedSet.insert(id);
                remove.push_back(id);
                finished++;
            }
        }

        for(auto id:remove)
        {
            processMap.erase(id);
        }

    }while(finished > 0);





    return processMap.size() == 0;

}

void ProcessControl::Run(std::shared_ptr<IData> data)
{

    try
    {

        std::vector<ProcessItem> processList;

        // convert and copy process map to a list, allows rerun .. Take copy of list
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            for(auto item:m_processes)
            {
                processList.push_back(item.second);
            }
        }

        std::atomic<int> activeThreads = 0;

        std::vector<std::thread> threads;
        std::map<std::string, std::future<std::string>>  futures;
        std::map<std::string, std::promise<std::string>> promises;

        // thread limit
        std::condition_variable threadsCv;
        std::mutex              threadsMutex;
        std::unique_lock        threadLock(threadsMutex);

        for(auto& processItem:processList)
        {
            std::string id = processItem.Id();
            auto &p = promises[id];
            futures[id] = p.get_future();
        }



        // reorder on priority
        std::sort(processList.begin(), processList.end());



        for(auto& processItem : processList)
        {
            std::string id = processItem.Id();

            auto &p = promises[id];


            threadsCv.wait(threadLock, [&activeThreads, this]{return activeThreads < m_maxThreads;});


            threads.emplace_back([&processItem, &p, &futures, &data, &activeThreads, &threadsCv]()
                                 {


                                     auto id             = processItem.Id();
                                     auto process        = processItem.Process();
                                     auto dependancies   = processItem.Dependancies();

                                     // wait till all dependancies are run
                                     for(auto item:dependancies)
                                     {
                                         auto &f = futures[item];
                                         f.wait();
                                     }


                                     activeThreads ++;
                                     threadsCv.notify_one();



                                     // now run, should limit amount ..
                                     if(!process->Run(data))
                                     {
                                         // not run
                                     }

                                     // set future done
                                     p.set_value(id);

                                     activeThreads --;
                                     threadsCv.notify_one();
                                 });
        }


        // Wait for all threads to finish
        for(auto& thread : threads)
        {
            thread.join();
        }

        threads.clear();
    }
    catch(...)
    {
        throw std::runtime_error("error");
    }

}
