//
// Created by Bradley Crouch on 15/04/2024.
//

#ifndef SIMPLETHREADMANAGER_IPROCESS_HPP
#define SIMPLETHREADMANAGER_IPROCESS_HPP

#include "IData.hpp"

class IProcess
{
public:
    virtual bool Run(std::shared_ptr<IData> data = nullptr) = 0;
    virtual ~IProcess() {};
};


#endif //SIMPLETHREADMANAGER_IPROCESS_HPP
