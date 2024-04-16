// *******************************************************
// UnitTest_ProcessControl.cpp
// *******************************************************
//
// Author: Bradley Crouch
// Copyright: © 2024 - April - 17
//
// Created by Bradley Crouch on 01/04/2024.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <set>

#include "../include/ProcessControl.hpp"


// Define a mock class for the IProcess interface
class MockProcess : public SimpleThreadManager::IProcess {
public:
    MOCK_METHOD(bool, Run, (std::shared_ptr<SimpleThreadManager::IData> data), (override));
};

TEST(ProcessControlTest, RunAndCheckProcessOrder) {

    // Create ProcessControl instance
    SimpleThreadManager::ProcessControl control(2);

    // Create mock processes
    std::shared_ptr<MockProcess> process1 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process2 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process3 = std::make_shared<MockProcess>();

    // Set expectations on mock processes
    {
        ::testing::InSequence seq;
        EXPECT_CALL(*process1, Run(testing::_)).Times(1);
        EXPECT_CALL(*process3, Run(testing::_)).Times(1);
        EXPECT_CALL(*process2, Run(testing::_)).Times(1);
    }

    // Add processes to ProcessControl
    control.AddProcess("A", process1);
    control.AddProcess("B", process2, {"C"});
    control.AddProcess("C", process3, {"A"});

    // run control
    control.Run();
}

TEST(ProcessControlTest, CheckDependenciesTrue) {

    // Create ProcessControl instance
    SimpleThreadManager::ProcessControl control(2);

    // Create mock processes
    std::shared_ptr<MockProcess> process1 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process2 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process3 = std::make_shared<MockProcess>();


    // Add processes to ProcessControl
    control.AddProcess("A", process1);
    control.AddProcess("B", process2, {"C"});
    control.AddProcess("C", process3, {"A"});

    // run control
    ASSERT_TRUE(control.CheckDependencies());
}


TEST(ProcessControlTest, CheckDependenciesFalse) {

    // Create ProcessControl instance
    SimpleThreadManager::ProcessControl control(2);

    // Create mock processes
    std::shared_ptr<MockProcess> process1 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process2 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process3 = std::make_shared<MockProcess>();


    // Add processes to ProcessControl
    control.AddProcess("A", process1);
    control.AddProcess("B", process2, {"C"});
    control.AddProcess("C", process3, {"B"});

    // run control
    ASSERT_FALSE(control.CheckDependencies());
}


TEST(ProcessControlTest, RunAndCheckThrowError) {

    // Create ProcessControl instance
    SimpleThreadManager::ProcessControl control(2);

    // Create mock processes
    std::shared_ptr<MockProcess> process1 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process2 = std::make_shared<MockProcess>();
    std::shared_ptr<MockProcess> process3 = std::make_shared<MockProcess>();


    // Add processes to ProcessControl
    control.AddProcess("A", process1);
    control.AddProcess("B", process2, {"C"});
    control.AddProcess("C", process3, {"B"});

    try{
        control.Run();
        FAIL() << "Expected std::invalid_argument";
    } catch (const std::invalid_argument& e) {
        EXPECT_STREQ("Invalid processing queue, deadlock detected", e.what());
    } catch (...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

