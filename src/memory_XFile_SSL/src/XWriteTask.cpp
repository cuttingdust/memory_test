#include "XWriteTask.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

class XWriteTask::PImpl
{
public:
    PImpl(XWriteTask *owenr);
    ~PImpl() = default;

public:
    XWriteTask  *owenr_ = nullptr;
    std::fstream ofs_;
};

XWriteTask::PImpl::PImpl(XWriteTask *owenr) : owenr_(owenr)
{
}

XWriteTask::XWriteTask()
{
    impl_ = std::make_unique<XWriteTask::PImpl>(this);
}

XWriteTask::~XWriteTask() = default;

auto XWriteTask::create() -> XWriteTask::Ptr
{
    return std::make_shared<XWriteTask>();
}

auto XWriteTask::init(const std::string &filepath) -> bool
{
    if (filepath.empty())
    {
        return false;
    }
    auto dir_path = fs::path(filepath).parent_path();

    if (!fs::exists(dir_path))
        fs::create_directories(dir_path);

    impl_->ofs_.open(filepath, std::ios::out | std::ios::binary); /// 二进制打开
    if (!impl_->ofs_)
    {
        std::cerr << "open file " << filepath << " failed!" << std::endl;
        return false;
    }
    std::cout << filepath << " open success!" << std::endl;
    return true;
}

auto XWriteTask::run() -> void
{
    std::cout << "============================" << std::endl;
    std::cout << "begin thread XWriteTask::Main" << std::endl;
    while (isRunning())
    {
        auto data = popFront();
        if (!data)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        impl_->ofs_.write(static_cast<char *>(data->data()), data->size());

        std::cout << "{" << data->size() << "}" << std::flush;
        if (data->isEnd())
        {
            break;
        }
    }
    std::cout << std::endl;
    impl_->ofs_.close();
    std::cout << "end thread XWriteTask::Main" << std::endl;
    std::cout << "============================" << std::endl;
}
