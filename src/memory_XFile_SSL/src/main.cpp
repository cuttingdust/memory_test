#include "XReadTask.h"
#include "XCryptTask.h"

#include <iostream>
#include <memory_resource>


int main(int argc, char *argv[])
{
    {
        std::cout << "目录下文件批量加密" << std::endl;
        /// 创建一个内存池
        std::shared_ptr<std::pmr::memory_resource> creator = std::make_shared<std::pmr::synchronized_pool_resource>();

        const auto rt = XReadTask::create();
        rt->init("assert/lena_hed.jpg");
        rt->setMemPool(creator);

        const auto ct = XCryptTask::create();
        rt->setNext(ct);

        rt->start();
        ct->start();

        rt->wait();
        ct->wait();
    }
    getchar();
    return 0;
}
