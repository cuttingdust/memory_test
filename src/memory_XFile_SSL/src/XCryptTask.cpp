#include "XCryptTask.h"

#include <iostream>
#include <thread>

XCryptTask::XCryptTask() = default;

XCryptTask::~XCryptTask() = default;

auto XCryptTask::create() -> XCryptTask::Ptr
{
    return std::make_shared<XCryptTask>();
}

auto XCryptTask::run() -> void
{
    std::cout << "========================" << std::endl;
    std::cout << "begin XCryptTask::Main()" << std::endl;
    while (isRunning())
    {
        const auto data = popFront();
        if (!data)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        std::cout << "<" << data->size() << ">" << std::flush;
    }
    std::cout << std::endl;
    std::cout << "end XCryptTask::Main()" << std::endl;
    std::cout << "========================" << std::endl;
}
