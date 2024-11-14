#include "../include/GameBoard.h"
#include <algorithm> 
#include <vector> 
#include <utility> 
#include <random>
#include <chrono>

GameBoard::GameBoard() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::mt19937(seed);
    val_dist = std::bernoulli_distribution(prob_4);
    index_dist = std::uniform_int_distribution<int>(0, board_size - 1);
    reset();
}

void GameBoard::add() {
    int val = val_dist(generator) ? 4 : 2;
    bool has_slot = false;
    for (auto &vec : board) {
        for (auto val : vec) {
            if (val == 0) {
                has_slot = true;
            }
        }
    }
    if (!has_slot) { return; }
    while (true) {
        int row = index_dist(generator);
        int col = index_dist(generator);
        if (board[row][col]) { continue; }
        board[row][col] = val;
        return;
    }
}

void GameBoard::reset() {
    // Initialize the board to be 4x4 with all values set to 0
    board.clear();
    std::vector<int> tmp;
    for (int i = 0; i < board_size; ++i) {
        tmp.push_back(0);
    }
    for (int i = 0; i < board_size; ++i) {
        board.push_back(tmp);
    }

    // Add two initial tiles
    add();
    add();
}

int GameBoard::move(Direction direction) {
    int tmp = slide_merge(direction);
    add();
    return tmp;
}

int GameBoard::update_row(std::vector<int>& vec) {
    int sum = 0;
    std::vector<int> tmp;
    while (true) {
        tmp.clear();
        // slide
        for (auto val : vec) {
            if (val) {
                tmp.push_back(val);
            }
        }
        // merge
        tmp.resize(vec.size());
        vec = tmp;
        bool flag = false;
        for (int i = 0; i < static_cast<int>(vec.size()) - 1; ++i) {
            if (vec[i] && vec[i] == vec[i + 1]) {
                vec[i] *= 2;
                vec[i + 1] = 0;
                sum += vec[i];
                flag = true;
            }
        }
        if (!flag) {  // 'merge' did nothing.
            break;
        }
    }
    return sum;
}

int GameBoard::slide_merge(Direction direction) {
    int sum = 0;
    switch (direction) {
        case Direction::UP:
            for (int i = 0; i < board_size; ++i) {
                std::vector<int> now;
                for (int j = 0; j < board_size; ++j) {
                    now.push_back(board[j][i]);
                }
                sum += update_row(now);
                for (int j = 0; j < board_size; ++j) {
                    board[j][i] = now[j];
                }
            }
            break;
        case Direction::DOWN:
            for (int i = 0; i < board_size; ++i) {
                std::vector<int> now;
                for (int j = board_size - 1; j >= 0; --j) {
                    now.push_back(board[j][i]);
                }
                sum += update_row(now);
                for (int j = board_size - 1; j >= 0; --j) {
                    board[j][i] = now[board_size - 1 - j];
                }
            }
            break;
        case Direction::LEFT:
            for (int i = 0; i < board_size; ++i) {
                std::vector<int> now;
                for (int j = 0; j < board_size; ++j) {
                    now.push_back(board[i][j]);
                }
                sum += update_row(now);
                for (int j = 0; j < board_size; ++j) {
                    board[i][j] = now[j];
                }
            }
            break;
        case Direction::RIGHT:
            for (int i = 0; i < board_size; ++i) {
                std::vector<int> now;
                for (int j = board_size - 1; j >= 0; --j) {
                    now.push_back(board[i][j]);
                }
                sum += update_row(now);
                for (int j = board_size - 1; j >= 0; --j) {
                    board[i][j] = now[board_size - 1 - j];
                }
            }
            break;
    }
    return sum;
}

bool GameBoard::isGameOver() const {
    #if 0
    // Check if there are any empty tiles
    for (auto &&vec : board) {
        for (auto val : vec) {
            if (val == 0) {
                return false;
            }
        }
    }
    return true;
    #else
    // check if any move can be done
    for (auto direction : {Direction::DOWN, Direction::LEFT, Direction::UP, Direction::RIGHT}) {
        GameBoard tmp = *this;
        tmp.move(direction);
        if (tmp.board != board) {
            return false;
        }
    }
    return true;
    #endif
}

bool GameBoard::hasWinningTile() const {
    for (auto &&vec : board) {
        for (auto val : vec) {
            if (val == winning_val) {
                return true;
            }
        }
    }
    return false;
}

const Board &GameBoard::getBoard() const {
    return board;
}