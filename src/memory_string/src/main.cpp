#include <iostream>

void TestString(std::string in_data, std::string out_data)
{
    std::cout << "in_data=" << in_data << std::endl;
    std::cout << in_data.size() << std::endl;
    std::cout << "in_data addr:" << (long long)in_data.data() << std::endl;
    out_data.resize(1024);
    std::cout << "out_data addr:" << (long long)out_data.data() << std::endl;
}

void TestRefString(std::string& in_data, std::string& out_data)
{
    std::cout << "in_data_ref =" << in_data << std::endl;
    std::cout << in_data.size() << std::endl;
    std::cout << "in_data_ref addr:" << (long long)in_data.data() << std::endl;
    std::cout << "out_data_ref addr:" << (long long)out_data.data() << std::endl;
    out_data.resize(1024); /// resize 地址会变
    std::cout << "out_data_ref addr:" << (long long)out_data.data() << std::endl;
}

int main(int argc, char* argv[])
{
    {
        std::cout << "cppds.com" << std::endl;

        std::string str1;
        str1.resize(1024);
        auto data = const_cast<char*>(str1.data());
        std::cout << "in_addr:" << (long long)data << std::endl;
        memset(data, 'A', 5);
        std::cout << str1 << std::endl;

        std::string out_str;
        std::string out_ref_str;
        std::cout << "out_addr:" << (long long)out_str.data() << std::endl;
        std::cout << "out_ref_addr:" << (long long)out_ref_str.data() << std::endl;

        TestString(str1, out_str);
        std::cout << "out_str size = " << out_str.size() << std::endl;
        std::cout << "out_str addr:" << (long long)out_str.data() << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "out_ref_addr:" << (long long)out_ref_str.data() << std::endl;

        TestRefString(str1, out_ref_str);
        std::cout << "out_ref_str size = " << out_ref_str.size() << std::endl;
        std::cout << "out_ref_str addr:" << (long long)out_ref_str.data() << std::endl;
    }

    getchar();
    return 0;
}
