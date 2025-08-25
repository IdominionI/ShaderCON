#pragma once
#include <vector>
#include <fstream>
#include <filesystem>

#include "FrameWork/AFW/Common/afw_definitions.h"

//static std::vector<char> read_file(const std::string& filename) {
static std::vector<char> read_file_into_char_vector(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}


class file_class {
public:
    file_class() {}
    ~file_class() {}


    //std::filesystem::path pathname;

    bool file_exists(std::string file) {
        std::filesystem::path pathname(file);
        return pathname_exists(pathname);
    }

    std::string get_absolute_pathname(std::string s_pathname) {
        std::filesystem::path pathname(s_pathname);
        return get_absolute_pathname(pathname);
    }

    std::string get_relative_pathname(std::string s_pathname) {
        std::filesystem::path pathname(s_pathname);
        return get_relative_pathname(pathname);
    }

    // --------------------------------------------------

    bool pathname_exists(std::filesystem::path pathname) {
        return std::filesystem::exists(pathname);
    }

    std::string get_absolute_pathname(std::filesystem::path pathname) {
        if (pathname.empty() || !std::filesystem::exists(pathname)) return "";

        if (pathname.is_absolute()) return pathname.string();

        return std::filesystem::absolute(pathname).string();
    }

    std::string get_relative_pathname(std::filesystem::path pathname) {
        if (pathname.empty() || !std::filesystem::exists(pathname)) return "";

        if (pathname.is_relative()) return pathname.string();

        return std::filesystem::relative(pathname).string();
    }

    // --------------------------------------------------
    bool read_text_file(std::string path_name, char_buffer_data_type &char_buffer) {
        return  read_file(path_name, char_buffer);
    }

    bool read_binary_file(std::string path_name, char_buffer_data_type &char_buffer) {
        return  read_file(path_name, char_buffer, true);
    }

    bool read_file(std::string path_name, char_buffer_data_type &char_buffer,bool binary_file = false ) {
        std::ifstream file;
        
        if(binary_file)     
            file.open(path_name, std::ios::ate | std::ios::binary);
        else
            file.open(path_name, std::ios::ate);

        if (!file.is_open()) {
            std::string msg = "INFO :: Cannot read file.\n File " + path_name + " does not exist. \n";
            printf(msg.c_str());
            std::runtime_error(msg.c_str());
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        char_buffer = buffer;

        return true;
    }

protected:

private:
};