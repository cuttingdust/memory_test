#include "XReadTask.h"

#include <fstream>
#include <iostream>
#include <filesystem>

class XReadTask::PImpl
{
public:
    PImpl(XReadTask *owenr);
    ~PImpl() = default;

public:
    XReadTask   *owenr_ = nullptr;
    std::fstream ifs_;
};

XReadTask::PImpl::PImpl(XReadTask *owenr) : owenr_(owenr)
{
}

XReadTask::XReadTask()
{
    impl_ = std::make_unique<XReadTask::PImpl>(this);
}

XReadTask::~XReadTask()
{
    if (impl_->ifs_.is_open())
    {
        impl_->ifs_.close();
    }
}

auto XReadTask::init(const std::string &filename) -> bool
{
    if (filename.empty())
    {
        return false;
    }

    /// 检查文件是否存在
    if (!std::filesystem::exists(filename))
    {
        std::cerr << "File does not exist: " << filename << std::endl;
        return false;
    }

    impl_->ifs_.open(filename, std::ios::in | std::ios::binary); /// 二进制打开
    if (!impl_->ifs_)
    {
        std::cerr << "open file " << filename << " failed!" << std::endl;
        return false;
    }
    std::cout << filename << " open success!" << std::endl;
    impl_->ifs_.seekg(0, std::ios::end);
    this->setDataByte(impl_->ifs_.tellg());
    impl_->ifs_.seekg(0, std::ios::beg);
    std::cout << " file size " << dataByte() << std::endl;
    return true;
}

auto XReadTask::run() -> void
{
    std::cout << "begin thread XReadTask::Main" << std::endl;
    while (isRunning())
    {
        if (impl_->ifs_.eof())
        {
            break;
        }

        char buf[10240] = { 0 };
        impl_->ifs_.read(buf, sizeof(buf));
        std::cout << "[" << impl_->ifs_.gcount() << "]" << std::flush;
    }
    std::cout << "end thread XReadTask::Main" << std::endl;
}
