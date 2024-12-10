#include "menu.h"


void printHeater() {
    std::cout << "Press\nnumber of the device to safely remove it\nEsc to exit\n";
    std::cout << "----------------------------\n";
}

void printMenu(std::wstring message) {
    std::cout << "Press:\n- number of the device to safely remove it\n- Esc to exit\n";
    std::cout << "----------------------------\n";
    if (!message.empty())
        std::wcout << message << "\n----------------------------\n";
    std::cout << "Connected devices:\n\n";
}