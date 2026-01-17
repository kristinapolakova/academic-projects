#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

char is_conteins(char sim) {
  std::unordered_set<char> set1 = {'b', 'f', 'p', 'v'};
  std::unordered_set<char> set2 = {'c', 'g', 'j', 'k', 'q', 's', 'x', 'z'};
  std::unordered_set<char> set3 = {'d', 't'};
  char set4 = 'l';
  std::unordered_set<char> set5 = {'m', 'n'};
  char set6 = 'r';
  std::unordered_set<char> set7 = {'a', 'e', 'i', 'o', 'u', 'y', 'h', 'w'};

  if (set1.count(sim)) {
    return '1';
  } else if (set2.count(sim)) {
    return '2';
  } else if (set3.count(sim)) {
    return '3';
  } else if (set4 == sim) {
    return '4';
  } else if (set5.count(sim)) {
    return '5';
  } else if (set6 == sim) {
    return '6';
  } else if (set7.count(sim)) {
    return '7';
  }

  return '0';
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Error: no argument\n";
    return 1;
  }

  std::string input = argv[1];
  const int SIZE_RESULT = 4;
  std::string result;

  if (!isalpha(input[0])) {
    result = "0000";
    std::cout << result << "\n";
    return 1;
  }

  result.push_back(std::toupper(input[0]));

  std::string rest = input.substr(1);
  std::string digits;
  std::transform(rest.begin(), rest.end(), std::back_inserter(digits),
                 [](char c) { return is_conteins(std::tolower(c)); });

  if (digits.find('0') != std::string::npos) {
    result = "0000";
    std::cout << result << "\n";
    return 1;
  }

  digits.erase(std::unique(digits.begin(), digits.end()), digits.end());
  digits.erase(std::remove(digits.begin(), digits.end(), '7'), digits.end());

  size_t num_digits = digits.size() > 3 ? 3 : digits.size();
  result += digits.substr(0, num_digits);
  result.append(SIZE_RESULT - result.size(), '0');
  std::cout << result << "\n";
}
