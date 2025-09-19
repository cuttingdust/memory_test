#include <functional>
#include <iostream>

class XData
{
public:
    static int count;
    int        index = 0;
    XData()
    {
        count++;
        index = count;
        std::cout << index << ":Create Data: " << this << std::endl;
    }
    ~XData()
    {
        std::cout << index << ":Drop Data: " << this << std::endl;
    }
};
int XData::count = 0;

void passByReference(std::shared_ptr<XData>& ptr)
{
    std::cout << __func__ << " { ptr.use_count() = " << ptr.use_count() << std::endl;
    std::cout << __func__ << " ptr->index = " << ptr->index << std::endl;
}

void passByConstReference(const std::shared_ptr<XData>& ptr)
{
    std::cout << __func__ << " { ptr.use_count() = " << ptr.use_count() << std::endl;
    std::cout << __func__ << " ptr->index = " << ptr->index << std::endl;
}

void passByValue(std::shared_ptr<XData> ptr)
{
    std::cout << __func__ << "{ ptr.use_count() =" << ptr.use_count() << std::endl;
}

void passByConstValue(const std::shared_ptr<XData> ptr)
{
    std::cout << __func__ << "{ ptr.use_count() =" << ptr.use_count() << std::endl;
}

void DelData(XData* p)
{
    std::cout << "call DelData" << std::endl;
    delete p;
}

/*
* shared_ptr
- 共享所有权的智能指针。多个shared_ptr指向同一对象
- 可复制构造 (CopyConstructible)
- 可复制赋值 (CopyAssignable)
- 数据访问非线程安全
- shared_ptr 的控制块是线程安全
*/
int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        /// 1 初步使用
        std::shared_ptr<int>   sp1(new int);
        std::shared_ptr<int[]> sp2(new int[1024]);
        *sp1   = 10;
        sp2[0] = 100;

        std::cout << "======1======" << std::endl;
        auto d1  = sp1.get();
        auto sp3 = std::make_shared<XData>();
        std::cout << "sp3.use_count() = " << sp3.use_count() << std::endl;

        /// 2 可复制构造 和赋值
        auto sp4 = sp3;
        std::cout << "sp4 = sp3 ; sp3.use_count() = " << sp3.use_count() << std::endl;
        std::cout << "sp4.use_count() = " << sp4.use_count() << std::endl;

        auto sp5 = std::make_shared<XData>();
        sp4      = sp5;
        std::cout << "sp4 = sp5 ; sp3.use_count() = " << sp3.use_count() << std::endl;
        {
            std::cout << "{ sp3.use_count() = " << sp3.use_count() << std::endl;
            auto sp6 = sp3;
            std::cout << "sp6 = sp3 ; sp3.use_count() = " << sp3.use_count() << std::endl;
        }
        std::cout << "} sp3.use_count() = " << sp3.use_count() << std::endl;
        sp3.reset(); /// -1 如果引用计数为0 则清理空间
        std::cout << "sp3.reset() sp3.use_count() = " << sp3.use_count() << std::endl;

        //////////////////////////////////////////////////////////////////////////////////////////
        /// 传递 是否影响引用计数 use_count
        auto sp7 = std::make_shared<XData>();
        std::cout << "sp7.use_count() = " << sp7.use_count() << std::endl;
        passByConstReference(sp7); ///
        std::cout << "} sp7.use_count() = " << sp7.use_count() << std::endl;

        passByConstValue(sp7);
        std::cout << "} sp7.use_count() = " << sp7.use_count() << std::endl;

        passByValue(sp7);
        std::cout << "} sp7.use_count() = " << sp7.use_count() << std::endl;

        ////////////////////////////////////////////////////////////////////////////////
        /// 3 shared_ptr定制删除函数
        {
            std::shared_ptr<XData> sp7(new XData, DelData);
            std::shared_ptr<XData> sp8(new XData,
                                       [](XData* p)
                                       {
                                           std::cout << "Call delete lambda" << std::endl;
                                           delete p;
                                       });
        }
        std::cout << "after delete }" << std::endl;

        /// 4 shared_ptr智能指针指向同一个对象不同成员
        {
            std::shared_ptr<XData> sc1(new XData);
            std::cout << "sc1 use_count=" << sc1.use_count() << std::endl;
            std::shared_ptr<int> sc2(sc1, &sc1->index);
            std::cout << "sc1 use_count=" << sc1.use_count() << std::endl;
            std::cout << "sc2 use_count=" << sc2.use_count() << std::endl;
            std::shared_ptr<int> sc3(sc1, &sc1->index);
            std::cout << "sc1 use_count=" << sc1.use_count() << std::endl;
            std::cout << "sc3 use_count=" << sc3.use_count() << std::endl;
        }

         std::cout << "======3======" << std::endl;
    }
    std::cout << "after }" << std::endl;
    std::cout << "======2======" << std::endl;

    getchar();
    return 0;
}
