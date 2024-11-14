#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstdio>
#include "../include/Game.h"
#include "../include/GameRender.h"
#include "../include/InputHandler.h"

#define debug(str) std::cout << (str) << std::endl;

int main(){
    Game game;

    // Display welcome message
    TerminalRender::displayWelcomeMessage();

    // Wait for the start game command
    InputType command;
    while (true) {
        command = InputHandler::handleInput();
        if (command == InputType::START_GAME) {
            break;
        } else {
            std::cout << "Invalid input. Press 'E' to start the game.\n";
        }
    }

    // Initialize the game
    game.initGame();
    debug("now");
    TerminalRender::clearScreen();
    TerminalRender::render(game.getGameBoard(), game.getScore(), game.getMoveCount());
    
    bool win = false;

    // Game loop
    while (true) {
        // Handle user input
        auto input = InputHandler::handleInput();
        switch (input) {
            case InputType::UP:
                game.updateGame(Direction::UP);
                break;
            case InputType::DOWN:
                game.updateGame(Direction::DOWN);
                break;
            case InputType::LEFT:
                game.updateGame(Direction::LEFT);
                break;
            case InputType::RIGHT:
                game.updateGame(Direction::RIGHT);
                break;
            case InputType::QUIT_GAME:
                std::cout << "Ending game." << std::endl;
                return 0;
            case InputType::UNDO:
                game.undoLastMove();
                break;
            default:
                std::cout << "Invalid input." << std::endl;
        }
        // std::cout << "Update finished" << std::endl;
        TerminalRender::clearScreen();
        TerminalRender::render(game.getGameBoard(), game.getScore(), game.getMoveCount());

        // Check for win or lose conditions
        if (game.hasWon()) {
            TerminalRender::displayWinMessage();
            win = true;
            break;
        }
        if (game.hasLost()) {
            TerminalRender::displayLoseMessage();
            break;
        }
    }

    if (!win) {return 0; }
    // Display elapsed time
    double time = game.getElapsedTime();
    int step = game.getMoveCount();
    int score = game.getScore();
    std::cout << "Time: " << std::setprecision(3) << time << "s" << std::endl;
    std::cout << "Step: " << step << std::endl;
    std::cout << "Score: " << score << std::endl;

    FILE *in = std::fopen("data", "r");
    double best_time = 1e100;
    int best_step = 10000000, best_score = -1000000;
    std::fscanf(in, "%lf%d%d", &best_time, &best_step, &best_score);
    std::fclose(in);
    best_time = std::min(time, best_time);
    best_step = std::min(step, best_step);
    best_score = std::max(score, best_score);
    
    FILE *out = std::fopen("data", "w");
    std::fprintf(out, "%.3lf\n%d\n%d\n", best_time, best_step, best_score);
    std::fclose(out);

    std::cout << std::endl;
    std::cout << "Best Time: " << std::setprecision(3) << best_time << "s" << std::endl;
    std::cout << "Best Step: " << best_step << std::endl;
    std::cout << "Best Score: " << best_score << std::endl;

    return 0;
}

