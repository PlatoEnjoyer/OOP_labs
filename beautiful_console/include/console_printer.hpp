#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>



// ANSI цвета
enum class Color {
    RESET = 0,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

class ConsolePrinter {
private:
    int x;
    int y;
    Color color;
    char symbol;
    std::map<char, std::vector<std::string>> fontMap;
    int fontHeight;

    // ANSI escape-последовательности как константы
    static constexpr const char* CSI = "\033[";
    static constexpr const char* MOVE_TO = "H";
    static constexpr const char* SET_COLOR = "m";
    static constexpr const char* CLEAR_LINE = "K";
    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* CLEAR = "\033[2J";
    static constexpr const char* HOME  = "\033[H";
public:
    ConsolePrinter(int x, int y, Color color, char symbol = '*') : x(x), y(y), color(color), symbol(symbol) {
        std::cout << CSI << y << ";" << x << MOVE_TO;
        loadFont(std::string(FONTS_DIR) + "/font5.txt");
    }

    ~ConsolePrinter() {
        std::cout << CSI << "0" << SET_COLOR;
    }

    void loadFont(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open font file: " << filename << std::endl;
            return;
        }

        fontMap.clear();
        fontHeight = 0;
        std::string line;
        char currentChar = '\0';
        std::vector<std::string> pattern;

        while (std::getline(file, line))
        {
            if (line.empty()) continue;

            if (line.length() == 1 && isalnum(line[0])) {
                    
                if (currentChar != '\0') {  
                    fontHeight = std::max(static_cast<size_t>(fontHeight), pattern.size());
                    fontMap[currentChar] = pattern;
                }

                currentChar = line[0];
                pattern.clear();
            } else {
                pattern.push_back(line);
            }
        }

        if (currentChar != '\0' && !pattern.empty()) {
            fontMap[currentChar] = pattern;
        }
    }

    void set_color(Color new_color) {
        this->color = new_color;
    }

    void printBig(const std::string& text) {
        std::cout << CSI << static_cast<int>(color) << SET_COLOR;
        int start_x = x;

        for (const auto& c : text) {
            auto it = fontMap.find(c);
            if (it == fontMap.end()) continue;
            
            const auto& rows = it->second;
            int height = static_cast<int>(rows.size());
            for (int i = 0; i < height; i++) {
                std::cout << CSI << (y + i) << ";" << x << MOVE_TO;
                std::string output;
                for (char ch: rows[i]) {
                    output += (ch == '*' ? std::string(1, symbol) : " ");                  
                }
                std::cout << output;
            }

            x += height * 1.5;
        }

        x = start_x;
        for (int i = 0; i < 1.7*fontHeight; i++) {
            std::cout << std::endl;
        }
    }

    static void print(const std::string& text, Color color, std::pair<int, int> pos, char symbol, const std::string& fontFile) {
        int x_pos = pos.first;
        int y_pos = pos.second;

        while (y_pos > 10)
        {
            std::cout << std::endl;
            y_pos -= 1;
        }
        
        ConsolePrinter temp(x_pos, y_pos, color, symbol);
        temp.loadFont(fontFile);
        temp.printBig(text);
    }

    static void reset_console() {
        std::cout << RESET << CLEAR << HOME << std::flush;
    }
};