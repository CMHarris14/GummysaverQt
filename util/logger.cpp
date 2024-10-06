#include "logger.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

const std::string Logger::LOG_PATH = "./logs/latest.log";

Logger::Logger() {}

void Logger::log(LogLevel level, const std::string &message) {
    check_log_file();
    std::ofstream logFile(LOG_PATH, std::ios::app);
    if (logFile.is_open()) {
        std::string levelStr = level_to_string(level);
        std::string timestamp = get_current_time();
        logFile << timestamp << " [" << levelStr << "] " << message
                << std::endl;
        logFile.close();
    } else {
        std::cerr << "Failed to open log file: " << LOG_PATH << std::endl;
    }
}

void Logger::cleanup_logs() {
    using namespace std::filesystem;

    path logDir = path(LOG_PATH).parent_path();
    std::vector<path> logFiles;

    for (const auto &entry : directory_iterator(logDir)) {
        if (is_regular_file(entry.status()) &&
            entry.path().extension() == ".log") {
            logFiles.push_back(entry.path());
        }
    }

    // Sort log files by modification time
    std::sort(logFiles.begin(), logFiles.end(),
              [](const path &a, const path &b) {
                  return last_write_time(a) < last_write_time(b);
              });

    // Remove log files older than 1 month
    auto now = std::chrono::system_clock::now();
    auto oneMonthAgo = now - std::chrono::hours(24 * 30);

    for (const auto &logFile : logFiles) {
        // Convert file_time_type to time_point<system_clock>
        auto fileTime = last_write_time(logFile);
        auto fileTimeInSysClock =
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                fileTime - std::filesystem::file_time_type::clock::now() +
                std::chrono::system_clock::now());

        if (fileTimeInSysClock < oneMonthAgo) {
            remove(logFile);
        }
    }

    // Keep only the 10 most recent log files
    if (logFiles.size() > 10) {
        for (size_t i = 0; i < logFiles.size() - 10; ++i) {
            remove(logFiles[i]);
        }
    }
}

void Logger::check_log_file() {
    using namespace std::filesystem;

    // Create directory if it doesn't exist
    path logDir = path(LOG_PATH).parent_path();
    if (!exists(logDir)) {
        create_directories(logDir);
    }

    // Check if the log file exists
    if (exists(LOG_PATH)) {
        // Get the last write time of the current log file
        auto lastWriteTime = last_write_time(LOG_PATH);

        // Annoying time type conversions
        auto lastWriteTimeSysClock =
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                lastWriteTime - file_time_type::clock::now() +
                std::chrono::system_clock::now());

        time_t fileTime =
            std::chrono::system_clock::to_time_t(lastWriteTimeSysClock);
        std::tm *fileTimeStruct = std::localtime(&fileTime);

        // Current date
        time_t now = std::time(nullptr);
        std::tm *currentTimeStruct = std::localtime(&now);

        // Check if log file was created today
        if (fileTimeStruct->tm_year == currentTimeStruct->tm_year &&
            fileTimeStruct->tm_mon == currentTimeStruct->tm_mon &&
            fileTimeStruct->tm_mday == currentTimeStruct->tm_mday) {
            return;
        }

        // Rename the log file to the expected format (MM-DD-YY.log)
        std::ostringstream oss;
        oss << std::put_time(fileTimeStruct, "%m-%d-%Y") << ".log";
        std::string newFileName = oss.str();

        rename(LOG_PATH, logDir / newFileName);
    }

    // Create a new empty log file
    std::ofstream logFile(LOG_PATH); // This creates the file
    logFile.close();                 // Close the file immediately
}

std::string Logger::get_current_time() {
    time_t now = time(nullptr);
    tm *localtm = localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localtm, "%H:%M");
    return oss.str();
}

std::string Logger::level_to_string(LogLevel level) {
    switch (level) {
    case INFO:
        return "INFO";
    case WARN:
        return "WARN";
    case ERROR:
        return "ERROR";
    default:
        return "";
    }
}
