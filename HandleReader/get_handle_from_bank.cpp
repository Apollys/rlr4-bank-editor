#include <iostream>

#include "bank_parser.hpp"
#include "rlr4_bank_gen.h"

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  os << "[";
  for (size_t i = 0; i < v.size(); i++) {
    os << v[i];
    if (i + 1 < v.size()) {
      os << ", ";
    }
  }
  os << (v.size() ? "]" : " ]");
  return os;
}

int main() {
  const std::string kInputBankFilename = "TestBank.SC2Bank";
  BankParser bank_parser(kInputBankFilename);
  bank_parser.Print();
  
  // FindResult find_result = test_bank_parser.Find("unit", "info");
  //std::cout << "Find result: " << find_result.found << ", " << find_result.value << std::endl;
  
  /*
  std::cout << "Encode test: " << std::endl;
  std::vector<int> test_ling_data{1, 72671, 0, 0, 100, 5, 5, 75, 160};
  std::string encoded_ling_data = bankgen::encode_runling_data(test_ling_data);
  std::cout << encoded_ling_data << std::endl << std::endl;
  */
  
  // Collect all runling data
  std::vector<std::vector<int>> all_ling_data;
  std::vector<std::string> slot_names{"01", "02", "03", "04", "05", "06", "07", "08"};
  for (const std::string& slot_name : slot_names) {
    FindResult find_result = bank_parser.Find("unit", slot_name);
    if (find_result.found) {
      all_ling_data.emplace_back(bankgen::decode_runling_data(find_result.value));
      std::cout << "unit " << slot_name << " data: " << all_ling_data.back() << std::endl;
    }
  }
  int character_checksum_no_handle = bankgen::compute_character_checksum(all_ling_data);
  // Collect all account data - this step unnecessary to just get the handle
  FindResult account_find_result = bank_parser.Find("account", "info");
  std::vector<int> account_data = bankgen::decode_account_data(account_find_result.value);
  std::cout << "Account data: " << account_data << std::endl;
  int account_checksum = bankgen::compute_account_checksum(account_data);
  // Decode the checksums from the string saved in "account" - "camera"
  FindResult checksum_find_result = bank_parser.Find("account", "camera");
  int decoded_account_checksum = 0;
  int decoded_character_checksum = 0;
  bankgen::decode_checksums(checksum_find_result.value, &decoded_account_checksum,
                            &decoded_character_checksum);
  // The character checksum with the handle is simply the character checksum plus the handle
  int handle = decoded_character_checksum - character_checksum_no_handle;
  std::cout << std::endl << "Handle: " << handle << std::endl << std::endl;
  
  return 0;
}

