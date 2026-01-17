#include "board.hpp"

void game_start_user_input(int& rows, int& cols, int& total_mines) {
  while (!(std::cin >> rows >> cols) || rows <= 0 || cols <= 0) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  int board_size = rows * cols;
  while (!(std::cin >> total_mines) || total_mines <= 0 ||
         total_mines >= board_size) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

void mines_hand_spawn(Board& board, int mines_count) {
  int row, col;
  for (int i = 0; i < mines_count; ++i) {
    while (true) {
      std::cin >> row >> col;
      if (row < 0 || row >= board.get_rows() || col < 0 ||
          col >= board.get_cols()) {
        std::cout << "Invalid coordinates. Try again.\n";
        continue;
      }
      if (board.has_mine(row, col)) {
        std::cout << "Mine already placed there. Choose another cell.\n";
        continue;
      }
      break;
    }
    board.set_mine(row, col);
  }
  board.set_total_mines(mines_count);
}

int main() {
  std::cout << "Hi! Welcome to the 'Mines' game!\n";

  Board board;
  int rows, cols, total_mines;

  game_start_user_input(rows, cols, total_mines);

  board.board_init(rows, cols);

  mines_hand_spawn(board, total_mines);

  board.print(std::cout);

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::string user_input;
  while (board.get_status() == GameStatus::RUN) {
    std::getline(std::cin, user_input);
    board.process_command(user_input);
    board.print(std::cout);
  }

  return 0;
}
