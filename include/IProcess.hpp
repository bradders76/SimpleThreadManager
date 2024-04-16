// *******************************************************
// IProcess.cpp
// *******************************************************
//
// Author: Bradley Crouch
// Copyright: © 2024 - April - 17
//
// Created by Bradley Crouch on 01/04/2024.
//

#ifndef SIMPLETHREADMANAGER_IPROCESS_HPP
#define SIMPLETHREADMANAGER_IPROCESS_HPP

#include "IData.hpp"
namespace SimpleThreadManager
{
    class IProcess
    {
    public:
        virtual bool Run(std::shared_ptr<IData> data = nullptr) = 0;
        virtual ~IProcess() {};
    };
}


#endif //SIMPLETHREADMANAGER_IPROCESS_HPP
