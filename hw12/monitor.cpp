#include "monitor.h"
#include <chrono>
#include <thread>
#include <filesystem>

FileMonitor::FileMonitor(const std::string &targetpath,
                         std::chrono::milliseconds interval,
                         const std::string &logfilename)
    : logger{logfilename}
    , interval{interval}
    , targetpath{targetpath}
    //, running{true}
{
    for (const auto &entry : std::filesystem::directory_iterator(targetpath)) {
        last_modified[entry.path().string()] = std::filesystem::last_write_time(entry);
    }
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds timeout) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time + timeout;

    while (std::chrono::high_resolution_clock::now() < end_time) {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(targetpath)) {
            auto path = entry.path().string();
            auto last_write_time = std::filesystem::last_write_time(entry);
            if (last_modified.count(path) == 0) {
                logger.log(path, status::added);
                last_modified[path] = last_write_time;
            } else if (last_modified[path] != last_write_time) {
                logger.log(path, status::changed);
                last_modified[path] = last_write_time;
            }
        }
        for (auto it = last_modified.begin(); it != last_modified.end(); ) {
            if (!std::filesystem::exists(it->first)) {
                logger.log(it->first, status::removed);
                it = last_modified.erase(it);
            } else {
                ++it;
            }
        }
        std::this_thread::sleep_for(interval);
    }
}





// void FileMonitor::start(std::chrono::seconds timeout) {
//     auto end = std::chrono::steady_clock::now() + timeout;
//     while (running && std::chrono::steady_clock::now() < end) {
//         for (const auto &entry : std::filesystem::directory_iterator(targetpath)) {
//             auto path = entry.path().string();
//             auto last_write_time = std::filesystem::last_write_time(entry);
//             if (last_modified.count(path) == 0) {
//                 last_modified[path] = last_write_time;
//                 logger.log("+ " + path, status::added);
//             } else if (last_modified[path] != last_write_time) {
//                 last_modified[path] = last_write_time;
//                 logger.log("! " + path, status::changed);
//             }
//         }
//         for (auto it = last_modified.begin(); it != last_modified.end();) {
//             if (!std::filesystem::exists(it->first)) {
//                 logger.log("~ " + it->first, status::removed);
//                 it = last_modified.erase(it);
//             } else {
//                 it++;
//             }
//         }
//         std::this_thread::sleep_for(interval);
//     }
// }