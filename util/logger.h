#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <string>

class Logger {
  public:
    Logger();

    enum LogLevel { INFO, WARN, ERROR };

    static void log(LogLevel level, const std::string &message);

    static void cleanup_logs();

  private:
    static const std::string LOG_PATH;

    static void check_log_file();
    static std::string get_current_time();
    static std::string level_to_string(LogLevel level);
};

#endif // LOGGER_H
