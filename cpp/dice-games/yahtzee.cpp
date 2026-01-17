#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>

int main() {
  std::array<int, 5> b;

  for (int i = 0; i < 5; i++) {
    std::cin >> b[i];
    if ((b[i] < 1) || (b[i] > 6)) {
      std::cout << "Error: invalid dice value\n";
      return 1;
    }
  }

  if (std::cin.get() != '\n') {
    std::cout << "Error: more than 5 values entered\n";
    return 1;
  }

  int countSL = 1;
  int maxSL = 1;
  int Three_four = 1;
  int maxTF = 1;
  int Full = false;

  int count1 = std::count(b.begin(), b.end(), 1) * 1;
  int count2 = std::count(b.begin(), b.end(), 2) * 2;
  int count3 = std::count(b.begin(), b.end(), 3) * 3;
  int count4 = std::count(b.begin(), b.end(), 4) * 4;
  int count5 = std::count(b.begin(), b.end(), 5) * 5;
  int count6 = std::count(b.begin(), b.end(), 6) * 6;

  int chance = std::accumulate(b.begin(), b.end(), 0);

  bool all =
      std::all_of(b.begin(), b.end(), [&](int val) { return val == b[0]; });

  for (int i = 0; i < 5; i++) {
    // если это заменять алгоритмами получается два цикла
    // дополнительных, поэтому так оставила

    if (i < 4 && (b[i] == b[i + 1])) {
      Three_four++;
    } else {
      if (Three_four > maxTF) {
        maxTF = Three_four;
      }
      Three_four = 1;
    }

    if ((b[i] + 1) == b[i + 1]) {
      countSL++;
      if (countSL > maxSL) {
        maxSL = countSL;
      } else {
        countSL = 1;
      }
    }
    // исходный вариант:
    // if ((b[0] == b[1]) && (b[0] != b[2]) && (b[2] == b[3]) && (b[2] == b[4]))
    // {
    //   Full = true;
    // }
    bool para = std::search_n(b.begin(), b.end(), 2, b[0]) == b.begin();
    bool troika =
        std::search_n(b.begin() + 2, b.end(), 3, b[2]) == b.begin() + 2;
    if (para && troika && b[0] != b[2]) {
      Full = true;
    }
  }

  if (Three_four > maxTF) {
    maxTF = Three_four;
  }

  int Kind_three = (maxTF >= 3) ? chance : 0;
  int Kind_four = (maxTF >= 4) ? chance : 0;
  int fullH = (Full) ? 25 : 0;
  int smallS = (maxSL >= 4) ? 30 : 0;
  int largeS = (maxSL == 5) ? 40 : 0;
  int yahtzee = (all) ? 50 : 0;

  std::cout << "Ones:             " << count1 << "\n";
  std::cout << "Twos:             " << count2 << "\n";
  std::cout << "Threes:           " << count3 << "\n";
  std::cout << "Fours:            " << count4 << "\n";
  std::cout << "Fives:            " << count5 << "\n";
  std::cout << "Sixes:            " << count6 << "\n";
  std::cout << "Three Of A Kind:  " << Kind_three << "\n";
  std::cout << "Four Of A Kind:   " << Kind_four << "\n";
  std::cout << "Full House:       " << fullH << "\n";
  std::cout << "Small Straight:   " << smallS << "\n";
  std::cout << "Large Straight:   " << largeS << "\n";
  std::cout << "Yahtzee:          " << yahtzee << "\n";
  std::cout << "Chance:           " << chance << "\n";
}