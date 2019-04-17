#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>

// Use ordered map here just because file size is small and ordering is convenient.
// Can easily change to unordered_map for performance if needed.
template <typename... Args>
using MapType = std::map<Args...>;

struct BankEntry {
  BankEntry() {}
  BankEntry(const std::string& key, const std::string& type, const std::string& value) :
            key(key), type(type), value(value) {}

  std::string key;
  std::string type;
  std::string value;
};

using BankSectionMap = MapType<std::string, BankEntry>;

struct FindResult {
  FindResult() : found(false) {}
  FindResult(bool found, const std::string& value) : found(found), value(value) {}

  bool found;
  std::string value;
};

class BankParser {
 public:
  BankParser(const std::string& input_filename) {
    std::ifstream bank_ifstream(input_filename);
    std::string line;
    std::string current_section;
    while (std::getline(bank_ifstream, line)) {
      // Check for bank version line
      std::string search_string = "<Bank version=\"";
      size_t find_result = line.find(search_string);
      if (find_result != std::string::npos) {
        bank_version_ = BankParser::ParseValue(line, find_result + search_string.length());
        continue;
      }
      // Check for section start
      search_string = "<Section name=\"";
      find_result = line.find(search_string);
      if (find_result != std::string::npos) {
        current_section = BankParser::ParseValue(line, find_result + search_string.length());
        continue;
      }
      // Check for key
      search_string = "<Key name=\"";
      find_result = line.find(search_string);
      if (find_result != std::string::npos) {
        std::string new_key = BankParser::ParseValue(line, find_result + search_string.length());
        // Each key line must be followed by a value line, not checking validity for simplicity
        std::getline(bank_ifstream, line);
        search_string = "<Value string=\"";  // assuming all values are string type for now
        size_t find_result = line.find(search_string);
        std::string new_value = BankParser::ParseValue(line, find_result + search_string.length());
        BankEntry new_entry(new_key, "string", new_value);
        bank_map_[current_section][new_key] = new_entry;
        continue;
      }
      // Check for signature
      search_string = "<Signature value=\"";
      find_result = line.find(search_string);
      if (find_result != std::string::npos) {
        bank_signature_ = BankParser::ParseValue(line, find_result + search_string.length());
        continue;
      }
    }
  }
  
  FindResult Find(const std::string& section, const std::string& key) {
    FindResult find_result;
    if (bank_map_.count(section) && bank_map_[section].count(key)) {
      find_result.found = true;
      find_result.value = bank_map_[section][key].value;
    }
    return find_result;
  }
  
  void Print() {
    std::cout << "Bank version: " << bank_version_ << std::endl;
    for (const auto& section_pair : bank_map_) {
      std::cout << "Section: " << section_pair.first << std::endl;
      for (const auto& entry_pair : section_pair.second) {
        const BankEntry& entry = entry_pair.second;
        std::cout << "   Key: " << entry.key << ", type: " << entry.type
                  << ", value: " << entry.value << std::endl;
      }
    }
    std::cout << "Signature: " << bank_signature_ << std::endl;
    std::cout << std::endl;
  }
  
 private:
  std::string bank_version_;
  MapType<std::string, BankSectionMap> bank_map_;
  std::string bank_signature_;
  
  // Given string and starting point, returns substring up to (excluding) the next "
  static std::string ParseValue(const std::string input_string, size_t start_index) {
    size_t find_result = input_string.find('"', start_index);
    if (find_result != std::string::npos) {
      size_t substring_length = find_result - start_index;
      return input_string.substr(start_index, substring_length);
    }
    // If not found, just return the rest of the string
    return input_string.substr(start_index);
  }
  
  
};
  
