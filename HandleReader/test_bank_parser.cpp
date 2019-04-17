#include <iostream>

#include "bank_parser.hpp"

int main() {
  const std::string kInputBankFilename = "TestBank.SC2Bank";
  BankParser test_bank_parser(kInputBankFilename);
  test_bank_parser.Print();
  
  FindResult find_result = test_bank_parser.Find("unit", "info");
  std::cout << "Find result: " << find_result.found << ", " << find_result.value << std::endl;
  
  return 0;
}

