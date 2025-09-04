#include <string>
#include <iostream>

int main() {
  std::string s1;
  std::cout << "What's your name? ";
  std::cin >> s1;
  std::cout << "Hello, " << s1 << "!" << std::endl;
}