#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <regex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <locale>
#include <codecvt>

// 1. Перечислитель LogLevel
enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

// Вспомогательная функция для преобразования LogLevel в строку
std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// 2. Абстрактный класс фильтров
class ILogFilter {
public:
    virtual ~ILogFilter() = default;
    virtual bool match(LogLevel log_level, const std::string& text) = 0;
};

// 3. Реализации фильтров

// SimpleLogFilter - фильтрация по вхождению текста
class SimpleLogFilter : public ILogFilter {
private:
    std::string pattern_;
public:
    explicit SimpleLogFilter(const std::string& pattern) : pattern_(pattern) {}
    
    bool match(LogLevel log_level, const std::string& text) override {
        return text.find(pattern_) != std::string::npos;
    }
};

// ReLogFilter - фильтрация по регулярному выражению
class ReLogFilter : public ILogFilter {
private:
    std::regex pattern_;
public:
    explicit ReLogFilter(const std::string& pattern) : pattern_(pattern) {}
    
    bool match(LogLevel log_level, const std::string& text) override {
        return std::regex_search(text, pattern_);
    }
};

// LevelFilter - фильтрация по уровню логирования
class LevelFilter : public ILogFilter {
private:
    LogLevel min_level_;
public:
    explicit LevelFilter(LogLevel min_level) : min_level_(min_level) {}
    
    bool match(LogLevel log_level, const std::string& text) override {
        return static_cast<int>(log_level) >= static_cast<int>(min_level_);
    }
};

// 4. Абстрактный класс обработчиков
class ILogHandler {
public:
    virtual ~ILogHandler() = default;
    virtual void handle(LogLevel log_level, const std::string& text) = 0;
};

// 6. Абстрактный класс форматтеров
class ILogFormatter {
public:
    virtual ~ILogFormatter() = default;
    virtual std::string format(LogLevel log_level, const std::string& text) = 0;
};

// 7. Реализация форматтера с временной меткой
class TimestampFormatter : public ILogFormatter {
public:
    std::string format(LogLevel log_level, const std::string& text) override {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y.%m.%d %H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        
        return "[" + logLevelToString(log_level) + "] [" + ss.str() + "] " + text;
    }
};

// 5. Реализации обработчиков

// ConsoleHandler - вывод в консоль
class ConsoleHandler : public ILogHandler {
private:
    std::string getColorCode(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "\033[32m";  // Зеленый
            case LogLevel::WARN: return "\033[33m";  // Желтый
            case LogLevel::ERROR: return "\033[31m"; // Красный
            default: return "\033[0m";               // Сброс
        }
    }
    
public:
    void handle(LogLevel log_level, const std::string& text) override {
        std::cout << getColorCode(log_level) << text << "\033[0m" << std::endl;
    }
};

// FileHandler - запись в файл
class FileHandler : public ILogHandler {
private:
    std::ofstream file_;
public:
    explicit FileHandler(const std::string& filename) {
        file_.open(filename, std::ios::app);
    }
    
    ~FileHandler() {
        if (file_.is_open()) {
            file_.close();
        }
    }
    
    void handle(LogLevel log_level, const std::string& text) override {
        if (file_.is_open()) {
            file_ << text << std::endl;
        }
    }
};

// SocketHandler - имитация отправки через сокет
class SocketHandler : public ILogHandler {
private:
    std::string address_;
    int port_;
public:
    SocketHandler(const std::string& address, int port) 
        : address_(address), port_(port) {}
    
    void handle(LogLevel log_level, const std::string& text) override {
        std::cout << "[SOCKET to " << address_ << ":" << port_ << "] " << text << std::endl;
    }
};

// SyslogHandler - имитация записи в системные логи
class SyslogHandler : public ILogHandler {
public:
    void handle(LogLevel log_level, const std::string& text) override {
        std::cout << "[SYSLOG] " << text << std::endl;
    }
};

// FtpHandler - имитация записи на FTP сервер
class FtpHandler : public ILogHandler {
private:
    std::string server_;
    std::string path_;
public:
    FtpHandler(const std::string& server, const std::string& path = "")
        : server_(server), path_(path) {}
    
    void handle(LogLevel log_level, const std::string& text) override {
        std::cout << "[FTP to " << server_ << path_ << "] " << text << std::endl;
    }
};

// 8. Основной класс Logger
class Logger {
private:
    std::vector<std::unique_ptr<ILogFilter>> filters_;
    std::vector<std::unique_ptr<ILogFormatter>> formatters_;
    std::vector<std::unique_ptr<ILogHandler>> handlers_;

public:
    // Добавление фильтров, форматтеров и обработчиков
    void addFilter(std::unique_ptr<ILogFilter> filter) {
        filters_.push_back(std::move(filter));
    }
    
    void addFormatter(std::unique_ptr<ILogFormatter> formatter) {
        formatters_.push_back(std::move(formatter));
    }
    
    void addHandler(std::unique_ptr<ILogHandler> handler) {
        handlers_.push_back(std::move(handler));
    }
    
    // Основной метод логирования
    void log(LogLevel log_level, const std::string& text) {
        // Применяем фильтры
        for (const auto& filter : filters_) {
            if (!filter->match(log_level, text)) {
                return; // Сообщение не прошло фильтр
            }
        }
        
        std::string formatted_text = text;
        
        // Применяем форматтеры
        for (const auto& formatter : formatters_) {
            formatted_text = formatter->format(log_level, formatted_text);
        }
        
        // Передаем обработчикам
        for (const auto& handler : handlers_) {
            handler->handle(log_level, formatted_text);
        }
    }
    
    // Удобные методы для разных уровней логирования
    void log_info(const std::string& text) {
        log(LogLevel::INFO, text);
    }
    
    void log_warn(const std::string& text) {
        log(LogLevel::WARN, text);
    }
    
    void log_error(const std::string& text) {
        log(LogLevel::ERROR, text);
    }
};


// 9. Демонстрация работы системы
int main() {
    // Настройка кодировки консоли
    
    // Создаем логгер
    Logger logger;
    
    // Добавляем фильтры (комментируем некоторые для демонстрации)
    logger.addFilter(std::make_unique<LevelFilter>(LogLevel::INFO)); // Фильтр по уровню
    // logger.addFilter(std::make_unique<SimpleLogFilter>("important")); // Фильтр по тексту
    // logger.addFilter(std::make_unique<ReLogFilter>("(error|warning|info)")); // Фильтр по regex
    
    // Добавляем форматтер с временной меткой
    logger.addFormatter(std::make_unique<TimestampFormatter>());
    
    // Добавляем обработчики
    logger.addHandler(std::make_unique<ConsoleHandler>());
    logger.addHandler(std::make_unique<FileHandler>("app.log"));
    logger.addHandler(std::make_unique<SocketHandler>("localhost", 514));
    logger.addHandler(std::make_unique<SyslogHandler>());
    logger.addHandler(std::make_unique<FtpHandler>("ftp.example.com", "/logs/"));
    
    std::cout << "=== Demonstration of Logging System ===" << std::endl;
    
    // Тестируем логирование
    logger.log_info("This is important information message for testing");
    logger.log_warn("This is important warning about possible problem");
    logger.log_error("This is important error message for demonstration");
    
    // Сообщения, которые не пройдут фильтры (если они включены)
    logger.log_info("This message will be processed (filters are simplified)");
    logger.log_info("Message with important keyword will be processed");
    
    std::cout << "\n=== Demonstration completed ===" << std::endl;
    
    return 0;
}