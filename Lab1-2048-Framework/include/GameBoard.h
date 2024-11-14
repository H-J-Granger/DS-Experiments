#ifndef INC_2048_GAMEBOARD_H
#define INC_2048_GAMEBOARD_H

#include <vector>
#include <random>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

using Board = std::vector<std::vector<int>>;

class GameBoard {
private:
    constexpr static double prob_4 = 0.25;
    constexpr static int board_size = 4;
    std::bernoulli_distribution val_dist;
    std::uniform_int_distribution<int> index_dist;
    constexpr static int winning_val = 2048;

public:
    GameBoard();
    void reset();
    int move(Direction direction);
    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] bool hasWinningTile() const;
    [[nodiscard]] const Board& getBoard() const;

private:
    Board board;
    std::mt19937 generator;
    int slide_merge(Direction direction);
    void add();  // add a new tile
    int update_row(std::vector<int>& vec_);  // slide and merge a row, returning value gained.
};

#endif //INC_2048_GAMEBOARD_H
