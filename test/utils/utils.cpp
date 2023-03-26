#include "utils.hpp"

std::string read_file(const std::string &test_dir, const std::string &file_name) {
    std::string file_path = test_dir + file_name;
    std::ifstream ifs(file_path.c_str());
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string file_content = ss.str();

    return file_content;
}
