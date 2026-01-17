#include "board.hpp"

void Board::board_init(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;
  total_mines_ = 0;
  total_empty_cells_ = (rows_ * cols_);

  cells_.resize(rows_);
  for (int i = 0; i < rows_; ++i) {
    cells_[i].resize(cols_);
    for (int j = 0; j < cols_; ++j) {
      cells_[i][j] = Cell{};
    }
  }
}

Board::Board(int rows,
             int cols,
             const std::vector<std::pair<int, int>>& mines_coords)
    : rows_(rows),
      cols_(cols),
      total_mines_(mines_coords.size()),
      total_empty_cells_((rows * cols) - total_mines_),
      cells_(rows, std::vector<Cell>(cols)) {
  for (const auto& coord : mines_coords) {
    cells_[coord.first][coord.second].has_mine_ = true;
  }
}

void Board::print(std::ostream& output) const {
  for (int r = 0; r < rows_; ++r) {
    for (int c = 0; c < cols_; ++c) {
      const auto& cell = cells_[r][c];
      if (cell.is_opened_) {
        if (cell.has_mine_) {
          output << "* ";
        } else if (cell.mines_count_ > 0) {
          output << cell.mines_count_ << " ";
        } else {
          output << ". ";
        }
      } else if (cell.has_flag_) {
        output << "F ";
      } else {
        output << "# ";
      }
    }
    output << "\n";
  }
}

void Board::recursive_open(int row, int col) {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
    return;
  Cell& cell = cells_[row][col];
  if (cell.is_opened_ || cell.has_flag_)
    return;

  cell.is_opened_ = true;
  if (cell.has_mine_)
    return;

  check_mines_around_count(row, col);

  if (cell.mines_count_ == 0) {
    for (int i = row - 1; i <= row + 1; ++i) {
      for (int j = col - 1; j <= col + 1; ++j) {
        if (i != row || j != col) {
          recursive_open(i, j);
        }
      }
    }
  }
}

void Board::open(int row, int col) {
  recursive_open(row, col);
  check_end_game_condition(row, col);
}

void Board::flag(int row, int col) {
  Cell& cell = cells_[row][col];
  cell.has_flag_ = !cell.has_flag_;
}

void Board::check_mines_around_count(int row, int col) {
  int mines_counter = 0;
  for (int i = row - 1; i <= row + 1; ++i) {
    for (int j = col - 1; j <= col + 1; ++j) {
      if (i >= 0 && i < rows_ && j >= 0 && j < cols_) {
        if (cells_[i][j].has_mine_) {
          ++mines_counter;
        }
      }
    }
  }
  cells_[row][col].mines_count_ = mines_counter;
}

void Board::check_end_game_condition(int row, int col) {
  Cell& cell = cells_[row][col];
  if (cell.has_mine_ && cell.is_opened_) {
    game_status_ = GameStatus::LOSE;
    std::cout << "You lose!\n";
    return;
  }
  int total_opened = 0;
  for (const auto& row_cells : cells_) {
    for (const Cell& c : row_cells) {
      if (c.is_opened_)
        ++total_opened;
    }
  }
  if (total_opened == total_empty_cells_) {
    game_status_ = GameStatus::WIN;
    std::cout << "You win!\n";
  }
}

void Board::process_command(const std::string& command) {
  if (game_status_ != GameStatus::RUN) {
    return;
  }
  std::string cmd;
  int r = -1, c = -1;
  std::istringstream iss(command);
  iss >> cmd;
  if (cmd == "exit") {
    game_status_ = GameStatus::EXIT;
    std::cout << "You exit game.\n";
    return;
  }
  iss >> r >> c;
  if (cmd == "open") {
    open(r, c);
  } else if (cmd == "flag") {
    flag(r, c);
  }
}
