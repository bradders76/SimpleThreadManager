//
// Created by Bradley Crouch on 15/04/2024.
//

#include <memory>
#include "../include/ProcessControl.hpp"

class Data : public SimpleThreadManager::IData {
    private:
        std::mutex m_mtx;
        std::map<std::string, std::string> m_stringData;
    public:
        void SetData(const std::string &id, const std::string &data) {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_stringData[id] = data;
        }

        void GetData(const std::string &id, std::string &data) {
            std::lock_guard<std::mutex> lock(m_mtx);
            data = m_stringData[id];
        }
};

std::mutex m_lock;

class ProcessTest : public SimpleThreadManager::IProcess {
    private:
        std::string m_id;
        long m_wait_ms;
        std::string m_output;
    public:

        ProcessTest(std::string id, long wait_ms, std::string output) : m_id(id), m_wait_ms(wait_ms), m_output(output) {

        }

        bool Run(std::shared_ptr<SimpleThreadManager::IData> data = nullptr) {
            std::this_thread::sleep_for(std::chrono::milliseconds(m_wait_ms));

            std::lock_guard<std::mutex> lock(m_lock);
            std::cout << m_id << " " << m_output << std::endl;

            std::shared_ptr<Data> dataTest = std::dynamic_pointer_cast<Data>(data);


            if (dataTest != nullptr) {
                dataTest->SetData(m_id, m_output);
            }

            return true;
        }
    };


int main(int argc, const char *argv[]) {


    SimpleThreadManager::ProcessControl control(2);

    std::shared_ptr<Data> data = std::make_shared<Data>();

    std::shared_ptr<SimpleThreadManager::IProcess> process1 = std::make_shared<ProcessTest>("A", 500, "test 1");
    std::shared_ptr<SimpleThreadManager::IProcess> process2 = std::make_shared<ProcessTest>("B", 10, "test 2");
    std::shared_ptr<SimpleThreadManager::IProcess> process3 = std::make_shared<ProcessTest>("C", 40, "test 3");
    std::shared_ptr<SimpleThreadManager::IProcess> process4 = std::make_shared<ProcessTest>("D1", 500, "test D1");
    std::shared_ptr<SimpleThreadManager::IProcess> process5 = std::make_shared<ProcessTest>("D2", 500, "test D2");
    std::shared_ptr<SimpleThreadManager::IProcess> process6 = std::make_shared<ProcessTest>("D3", 500, "test D3");
    std::shared_ptr<SimpleThreadManager::IProcess> process7 = std::make_shared<ProcessTest>("D4", 500, "test D4");

    control.AddProcess("E",
                       [](const std::shared_ptr<SimpleThreadManager::IData>& pIData) {
                           std::shared_ptr<Data> pData = std::dynamic_pointer_cast<Data>(pIData);
                           if (pData)
                               pData->SetData("TEST", "TEST aaa");


                       },
                       {"A"});

    control.AddProcess("A", process1);
    control.AddProcess("B", process2, {"A"});
    control.AddProcess("C", process3, {"A"});
    control.AddProcess("D1", process4, {"B", "A"}, 2);
    control.AddProcess("D2", process5, {"C", "A"}, 2);
    control.AddProcess("D3", process6, {"C", "A"}, 1);
    control.AddProcess("D4", process7, {"C", "A"}, 1);

    std::cout << control.CheckDependiences() << std::endl;

    try {
        control.Run(data);
        control.Run();
    } catch (...) {

    }
    std::cout << "Hello, World!\n";
    return 0;
}



