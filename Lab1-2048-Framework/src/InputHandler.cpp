#include "../include/InputHandler.h"
#include "../include/conio.h" // For _getch()
#include <iostream>

InputType InputHandler::handleInput() {
    while (true) {
        int input = getch();

        // Filter out unwanted characters like new line or space
        if (input == '\n' || input == '\r' || input == ' ') {
            continue; // Skip unwanted characters
        }

        // Cast input to unsigned char and convert to uppercase
        input = static_cast<unsigned char>(toupper(input));

        switch (input) {
            case 'W': case 'w':
                return InputType::UP;
            case 'S': case 's':
                return InputType::DOWN;
            case 'A': case 'a':
                return InputType::LEFT;
            case 'D': case 'd':
                return InputType::RIGHT;
            case 'E': case 'e':
                return InputType::START_GAME;
            case 'Q': case 'q':
                return InputType::QUIT_GAME;
            case 'U': case 'u':
                return InputType::UNDO;
            default:
                return InputType::INVALID;
        }
    }
}
