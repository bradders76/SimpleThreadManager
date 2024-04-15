//
// Created by Bradley Crouch on 15/04/2024.
//

#ifndef SIMPLETHREADMANAGER_PROCESSFUNCTION_H
#define SIMPLETHREADMANAGER_PROCESSFUNCTION_H


#include <mutex>
#include <memory>
#include <functional>

#include "../include/IData.hpp"
#include "../include/IProcess.hpp"

class ProcessFunction : public IProcess
{
private:
    std::mutex m_lock;
    std::string m_id;
    std::function<void(std::shared_ptr<IData>)> m_function;

public:

    ProcessFunction(std::string id,  std::function<void(std::shared_ptr<IData>)> function): m_id(id), m_function(function)
    {

    }

    bool Run(std::shared_ptr<IData> data = nullptr);
};


#endif //SIMPLETHREADMANAGER_PROCESSFUNCTION_H
