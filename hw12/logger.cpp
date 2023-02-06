#include "logger.h"
#include <ctime>
#include <chrono>
#include <filesystem>
#include <iostream>


Logger::Logger(const std::string &filename) : file{} {
    std::filesystem::path old_log_file = filename + "_old";
    if (std::filesystem::exists(old_log_file)) {
        std::filesystem::remove(old_log_file);
        std::filesystem::rename(filename, old_log_file);
    } else if (std::filesystem::exists(filename)) {
        std::filesystem::rename(filename, old_log_file);
    }
    file.open(filename);
    if (!file.is_open()) {
    throw std::runtime_error("File could not be opened!");
    }
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << "Commencing logging for directory: "
         << std::filesystem::current_path()
         << std::endl
         << "Current time is: "
         << std::ctime(&time)
         << std::endl;
}

Logger::~Logger() {
    file.close();
    // explicit file closing needed in Windows
}

void Logger::log(const std::string &path, status what) const {
    if (not file.is_open()) {
        throw std::runtime_error("File could not be opened!");
    }

    switch (what) {
    case status::added :
        file << "+ File was added:    " << path << std::endl;
        break;
    case status::changed :
        file << "! File was modified: " << path << std::endl;
        break;
    case status::removed :
        file << "~ File was deleted:  " << path << std::endl;
        break;
    default:
        file << "? Error! Unknown file status detected for: " << path << std::endl;
        break;
    }
}
