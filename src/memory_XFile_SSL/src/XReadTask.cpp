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

auto XReadTask::create() -> XReadTask::Ptr
{
    return std::make_shared<XReadTask>();
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
    std::cout << "============================" << std::endl;
    std::cout << "begin thread XReadTask::Main" << std::endl;
    while (isRunning())
    {
        if (impl_->ifs_.eof())
        {
            break;
        }

        // char buf[10240] = { 0 };

        /// 创建内存池空间管理对象
        auto data      = XData::make(memPool());
        int  data_size = 1024;

        /// 申请空间
        void *buf = data->allocate(data_size);

        impl_->ifs_.read(static_cast<char *>(buf), data_size);
        if (impl_->ifs_.gcount() <= 0)
        {
            break;
        }
        data->setSize(impl_->ifs_.gcount());
        if (impl_->ifs_.eof())
        {
            data->setEnd(true);
        }
        std::cout << "[" << impl_->ifs_.gcount() << "]" << std::flush;

        if (next())
        {
            next()->pushBack(data);
        }
    }
    std::cout << std::endl;
    std::cout << "end thread XReadTask::Main" << std::endl;
    std::cout << "============================" << std::endl;
}
