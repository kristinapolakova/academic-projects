#ifndef SRC_BOARD_HPP
#define SRC_BOARD_HPP

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

enum class GameStatus { RUN, WIN, EXIT, LOSE };

struct Cell {
  bool is_opened_ = false;
  bool has_flag_ = false;
  bool has_mine_ = false;
  size_t mines_count_ = 0;
};

class Board {
 public:
  Board() = default;
  void board_init(int rows, int cols);
  Board(int rows,
        int cols,
        const std::vector<std::pair<int, int>>& mines_coords);
  void open(int row, int col);
  void flag(int row, int col);
  void print(std::ostream& output) const;
  GameStatus get_status() const { return game_status_; }
  void process_command(const std::string& command);

  int get_rows() const { return rows_; }
  int get_cols() const { return cols_; }
  bool has_mine(int row, int col) const { return cells_[row][col].has_mine_; }
  void set_mine(int row, int col) { cells_[row][col].has_mine_ = true; }
  void set_total_mines(int mines) {
    total_mines_ = mines;
    total_empty_cells_ = (rows_ * cols_) - total_mines_;
  }

 private:
  int rows_ = 0;
  int cols_ = 0;
  int total_mines_ = 0;
  int total_empty_cells_ = 0;
  std::vector<std::vector<Cell>> cells_;
  GameStatus game_status_ = GameStatus::RUN;

  void check_mines_around_count(int row, int col);
  void recursive_open(int row, int col);
  void check_end_game_condition(int row, int col);
};

void game_start_user_input(int& rows, int& cols, int& total_mines);

#endif
