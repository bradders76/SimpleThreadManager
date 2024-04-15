//
// Created by Bradley Crouch on 15/04/2024.
//

#include "../include/ProcessFunction.hpp"


bool ProcessFunction::Run(std::shared_ptr<IData> data)
{
    std::lock_guard<std::mutex> lock(m_lock);

    if(data == nullptr)
    {
        m_function(std::make_shared<IData>());

    }
    else
    {
        m_function(data);
    }

    return true;

}