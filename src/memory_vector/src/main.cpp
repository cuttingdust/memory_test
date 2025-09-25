#include <iostream>
#include <vector>
/// 使用vector 传递和获取内存
/// 功能等同于string
/// 可用传递不同的类型
/// 可以用作返回值支持move
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
    XData(const XData& other)
    {
        index = other.index;
        std::cout << index << ":Copy Data: " << this << std::endl;
    }

    XData(XData&& other) noexcept
    {
        index       = other.index;
        other.index = 0;
        std::cout << index << ":Move Data: " << this << std::endl;
    }

    XData& operator=(const XData& other)
    {
        if (this != &other)
        { /// 防止自我赋值
            index = other.index;
            std::cout << index << ": Copy Assignment Data: " << this << std::endl;
        }
        return *this;
    }

    XData& operator=(XData&& other) noexcept
    {
        if (this != &other)
        { /// 防止自我赋值
            index       = other.index;
            other.index = 0;
            std::cout << index << ": Move Assignment Data: " << this << std::endl;
        }
        return *this;
    }

    ~XData()
    {
        std::cout << index << ":Drop Data: " << this << std::endl;
    }
};
int XData::count = 0;

std::vector<XData> TestVecByValue(std::vector<XData> data)
{
    std::cout << "TestVecByValue data:" << data.data() << std::endl;
    std::vector<XData> re;
    // re.resize(3);
    // std::cout << "re.data()" << re.data() << std::endl;
    // std::cout << "re Byte=" << re.size() * sizeof(XData) << std::endl;

    return re;
}

std::vector<XData> TestVecByReferce(const std::vector<XData>& data)
{
    std::cout << "TestVecByReferce data:" << data.data() << std::endl;
    std::vector<XData> re;
    re.resize(3);
    std::cout << "re.data()" << re.data() << std::endl;
    std::cout << "re Byte=" << re.size() * sizeof(XData) << std::endl;

    return re;
}

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;
        std::vector<XData> vd{ 3 };

        std::cout << "vd.data() = " << vd.data() << std::endl;
        auto rd = TestVecByValue(vd);
        std::cout << "rd.data()" << rd.data() << std::endl;

        std::cout << "==========================================" << std::endl;
        std::cout << "vd.data() = " << vd.data() << std::endl;
        auto rd2 = TestVecByReferce(vd);
        std::cout << "rd2.data()" << rd2.data() << std::endl;
    }

    getchar();
    return 0;
}
