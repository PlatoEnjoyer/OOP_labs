#include "console_printer.hpp"



int main() {
    std::string font5_path = std::string(FONTS_DIR) + "/font5.txt";
    std::string font7_path = std::string(FONTS_DIR) + "/font7.txt";

    ConsolePrinter::print("HELLO", Color::GREEN, {5, 5}, '#', font7_path);

    ConsolePrinter::print("WORLD", Color::BLUE, {50, 50}, '#', font5_path);

    ConsolePrinter printer1(7, 5, Color::YELLOW);
    ConsolePrinter printer2(8, 4, Color::RED);

    printer1.loadFont(font7_path);
    printer1.printBig("HELLO");
    printer2.printBig("WORLD");
    printer1.set_color(Color::CYAN);
    printer1.printBig("BYE");
    printer1.set_color(Color::MAGENTA);
    printer1.printBig("WORLD");

    ConsolePrinter::reset_console();
}