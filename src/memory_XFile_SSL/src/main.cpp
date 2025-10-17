#include "XFileCrypt.h"
#include <argparse/argparse.hpp>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

static auto processFile(const fs::path& input, const fs::path& output, const std::string& password, bool encrypt)
        -> void
{
    auto xfc = XFileCrypt::create();
    xfc->start(input.string(), output.string(), password, encrypt);
    xfc->wait();

    if (encrypt)
    {
        std::cout << "加密文件: " << input << " -> " << output << std::endl;
    }
    else
    {
        std::cout << "解密文件: " << input << " -> " << output << std::endl;
    }
}

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("FileCrypt");

    program.add_argument("-i", "--input").help("input file or directory path").required();
    program.add_argument("-o", "--output").help("output encrypted file or encrypted directory path").required();
    program.add_argument("-p", "--password").help("password for encryption/decryption").required();
    program.add_argument("-de", "--decrypt")
            .help("encrypt flag (use this flag for decryption, omit for encryption)")
            .default_value(false)
            .implicit_value(true);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "错误: " << e.what() << std::endl;
        std::cerr << "使用 -h 或 --help 查看帮助信息。" << std::endl;
        std::cerr << program; /// 输出帮助信息
        return 1;
    }

    const auto& in  = program.get<std::string>("--input");
    const auto& out = program.get<std::string>("--output");
    const auto& pwd = program.get<std::string>("--password");
    const auto& en  = program.get<bool>("--decrypt");

    auto in_path  = fs::path(in);
    auto out_path = fs::path(out);

    if (!fs::exists(in_path))
    {
        std::cerr << "输入路径不存在: " << in_path << std::endl;
        return -2; /// 返回错误代码
    }

    if (fs::is_directory(in_path))
    {
        std::cout << "输入是一个目录: " << in_path << std::endl;

        for (const auto& entry : fs::directory_iterator(in_path))
        {
            if (fs::is_regular_file(entry.path()))
            {
                const auto& file_path   = entry.path();
                auto        file_name   = file_path.stem();      /// 不包括扩展名
                auto        file_suffix = file_path.extension(); /// 包括扩展名

                auto out_file_name = file_name.string() + "_out" + file_suffix.string();
                auto out_file_path = out_path / out_file_name;

                processFile(file_path, out_file_path, pwd, en);
            }
        }
    }
    else if (fs::is_regular_file(in_path))
    {
        processFile(in_path, out_path, pwd, en);
    }
    else
    {
        std::cerr << "输入路径既不是文件也不是目录: " << in_path << std::endl;
        return -1; /// 返回错误代码
    }

    return 0; /// 正常结束
}
