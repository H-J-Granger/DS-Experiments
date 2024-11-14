#include "../include/Game.h"

Game::Game() = default;

void Game::initGame() {
    gameBoard.reset();
    score = 0;
    moveCount = 0;
    historyGameBoards = std::stack<GameBoard>();
    startTime = std::chrono::system_clock::now();
}

void Game::updateGame(Direction direction) {
    historyGameBoards.push(gameBoard);
    int delta = gameBoard.move(direction);
    historyScoreDelta.push(delta);
    moveCount++;
    score += delta;
}

void Game::undoLastMove() {
    if (historyGameBoards.empty() || historyScoreDelta.empty()) { return; }
    gameBoard = historyGameBoards.top();
    historyGameBoards.pop();
    score -= historyScoreDelta.top();
    historyScoreDelta.pop();
    moveCount--;
}

bool Game::hasWon() const {
    return gameBoard.hasWinningTile();
}

bool Game::hasLost() const {
    return gameBoard.isGameOver();
}

double Game::getElapsedTime() const {
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    return elapsedSeconds.count();
}

int Game::getScore() const {
    return score;
}

int Game::getMoveCount() const {
    return moveCount;
}

const GameBoard& Game::getGameBoard() const {
    return gameBoard;
}