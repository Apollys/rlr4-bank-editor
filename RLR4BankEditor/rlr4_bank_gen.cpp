#include "rlr4_bank_gen.h"
#include "rlr4_consts.h"
#include "sha1.h"

/* Generic helper methods */

std::string bankgen::vtos(std::vector<int> &arr) {
    std::string s = "[";
    for (auto el: arr) {
        s += std::to_string(el);
        s += ", ";
    }
    if (s.length() > 1) {
        s = s.substr(0, s.length()-2);
        s += "]";
        return s;
    }
    else {
        return "[]";
    }
}

std::string bankgen::svtos(std::vector<std::string> &arr) {
    std::string s;
    for (unsigned int i = 0; i+1 < arr.size(); i++) {
        s += arr[i] + ", ";
    }
    if (arr.size() > 0) {
        s += arr[arr.size()-1];
    }
    return s;
}

int bankgen::strings_equal(std::string first, std::string second) {
    return first.compare(second) == 0;
}

int bankgen::string_startswith(std::string s, std::string prefix) {
    std::string s_start = s.substr(0, prefix.length());
    return bankgen::strings_equal(s_start, prefix);
}

std::string bankgen::str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
                   [](unsigned char c){ return toupper(c); } );
    return s;
}



/**
RLR 4 Bank File Structure

Main sections:
 - Unit: contains all data related to the runlings
    -> one string for each slot, if you have a runling saved there
    -> one string "info" to save which slots are open and which are used
 - Account: contains all remaining global data
    -> string "info" saving all the data
    -> string "camera" saving a checksum which is validated whenever the bank is loaded
 - Signature: basic algorithm by Blizzard, already cracked by Starbank etc
    -> function of: bank text, player handle, map creator handle (I think?)
    
Sample bank:

<?xml version="1.0" encoding="utf-8"?>
<Bank version="1">
    <Section name="unit"> // in section unit, all the runling data is saved
        <Key name="02">  // Slot 2
            <Value string=" XBzIkO5.68Q|#q/T$Cy=xSV"/>
        </Key>
        <Key name="01">  // Slot 1
            <Value string="/V=RG#k9l?`WYCz_B6G6T_!G"/>
        </Key>
        <Key name="08">
            <Value string=";ihez8$nd/S|BW=P{yW{ItxE"/>
        </Key>
        <Key name="04">
            <Value string=";iiS}]Nn- r$]G|(qwichq.{"/>
        </Key>
        <Key name="07">
            <Value string=";ih@eI41-dD{L((zR~57VQtM"/>
        </Key>
        <Key name="06">
            <Value string=";iiDnPbfqrY?;9Dl=s.?;jyl"/>
        </Key>
        <Key name="info">  // Slots information
            <Value string="/*qunHr 7h"/>
        </Key>
        <Key name="05">
            <Value string=";ihaG)`cg2EMRqKA?ewi}9(E"/>
        </Key>
    </Section>
    <Section name="account">
        <Key name="camera">
            <Value string="1/nN%;Y"/>
        </Key>
        <Key name="info">
            <Value string=";4gEKoV:Y`.*u+Xt8[z^|.jhW~P}r)%AR74fRNMdh(.l(It}MM#?sC6(R??TyfCc]T2h"/>
        </Key>
    </Section>
    <Signature value="7E3802175AC1253C849A19C2299BFE9D0ED1CAED"/>
</Bank>

**/

int bankgen::calc_points_used(int regen, int energy, int speed, int first_skill, int second_skill) {
    int sum = 0;
    sum += (regen > 100) ? 2*regen-100 : regen;
    sum += (energy > 100) ? 2*energy-100 : energy;
    sum += (speed > 100) ? 2*speed-100 : speed;
    sum += (first_skill + second_skill) * 4;
    return sum;
}

int bankgen::calc_points_used(std::vector<int> data) {
    return calc_points_used(data[2], data[3], data[4], data[5], data[6]);
}

std::string bankgen::encode_account_data(std::vector<int> &data) {
    assert(data.size() == rlr4::account_data_maxvals.size());
    starcode.start_code();
    for (unsigned int i = 0; i < data.size(); i++) {
        int val = data[i];
        int max_val = rlr4::account_data_maxvals[i];
        starcode.store_int(val, max_val);
    }
    std::string compressed_code = starcode.compress(starcode.get_code());
    return starcode.encrypt(compressed_code, rlr4::key);
}
    
std::string bankgen::encode_runling_data(std::vector<int> &data) {
    assert(data.size() == rlr4::runling_data_maxvals.size());
    starcode.start_code();
    for (unsigned int i = 0; i < data.size(); i++) {
        int val = data[i];
        int max_val = rlr4::runling_data_maxvals[i];
        starcode.store_int(val, max_val);
    }
    std::string compressed_code = starcode.compress(starcode.get_code());
    return starcode.encrypt(compressed_code, rlr4::key);
}

// Input vector should be 8 values, each of which is either 1 or 0
std::string bankgen::encode_slots_data(std::vector<int> &data) {
    assert(data.size() == 8);
    starcode.start_code();
    for (unsigned int i = 0; i < data.size(); i++) {
        int val = data[i];
        int max_val = 425 + i;
        starcode.store_int(val, max_val);
    }
    std::string compressed_code = starcode.compress(starcode.get_code());
    return starcode.encrypt(compressed_code, rlr4::key);
}

// The result of this function is saved into the "camera" string in "account" section of bank
std::string bankgen::encode_checksums(int account_checksum, int character_checksum) {
    starcode.start_code();
    starcode.store_int(account_checksum, 99000000);
    starcode.store_int(character_checksum, 98000000);
    std::string compressed_code = starcode.compress(starcode.get_code());
    return starcode.encrypt(compressed_code, rlr4::key);
}

// sum(total saves, normal/hard/time games, normal/hard/time wins, 
// bot/odin/diablo kills, total score, timemode best score, minigame high score, total deaths)
int bankgen::compute_account_checksum(std::vector<int> &account_data) {
    std::vector<unsigned int> indices{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 13, 14, 15, 16};
    int sum = 0;
    for (unsigned int i: indices) {
        sum += account_data[i];
    }
    return sum;
}

// sum over all slots 1-8 of (remaining points, regen, speed, energy, skill#1, skill#2, exp)
// handle is added in also, set handle = 0 to ignore handle in calculation
int bankgen::compute_character_checksum(std::vector<std::vector<int>> &all_character_data, int handle) {
    std::vector<unsigned int> indices{1, 2, 3, 4, 5, 6, 8};
    int sum = 0;
    for (std::vector<int> runling_data: all_character_data) {
        for (unsigned int i: indices) {
            sum += runling_data[i];
        }
    }
    return sum + handle;
}

// bank_tags: each element of bank_tags corresponds to one section in the bank
// if section_tags = bank_tags[i], section tags then contains the structure:
//  - section_tags[0] = {section_name} (single element vector)
//  - section_tags[1+] = {key_name, "Value", type, value} (always 4 element vector, this order)
std::string bankgen::compute_signature(int player_handle, int server, std::vector<std::vector<std::vector<std::string>>> bank_tags) {
    std::string to_hash("");
    // We start by concatenating the author handle, player handle, and bank name
    std::string handle_prefix;
    int author_handle;
    switch (server) {
        case rlr4::REGION_NA:
            handle_prefix = rlr4::handle_prefix_NA;
            author_handle = rlr4::author_handle_NA;
            break;
        case rlr4::REGION_EU:
            handle_prefix = rlr4::handle_prefix_EU;
            author_handle = rlr4::author_handle_EU;
            break;
    }
    to_hash += handle_prefix + std::to_string(author_handle);
    to_hash += handle_prefix + std::to_string(player_handle);
    to_hash += rlr4::bank_filename;
    // Next, we need to iterate through the sections in sorted (ascii value) order
    std::sort(bank_tags.begin(), bank_tags.end());
    for (std::vector<std::vector<std::string>> section_tags: bank_tags) {
        // the first item contains only the section name, we start by appending that
        to_hash += section_tags[0][0];
        // next, we need to iterate through the keys in sorted order
        std::sort(section_tags.begin()+1, section_tags.end());
        // now we append this sorted data
        for (unsigned int i = 1; i < section_tags.size(); i++) {
            for (std::string t: section_tags[i]) {
                to_hash += t;
            }
        }
    }
    std::string hash = sha1::sha1(to_hash);
    return bankgen::str_toupper(hash);
}

std::vector<std::string> bankgen::make_key_vec(std::string name, std::string type, std::string value) {
    /*
    inputString.Append(key.Name);
    inputString.Append("Value");
    inputString.Append(key.Type);
    inputString.Append(key.Value);
    */
    std::vector<std::string> results;
    results.push_back(name);
    results.push_back("Value");
    results.push_back(type);
    results.push_back(value);
    return results;
}

// Slot numbers contains corresponding slot number (1-indexed) for each runling in runling_data
std::string bankgen::generate_bank(std::vector<int> account_data, std::vector<std::vector<int>> runling_data,
                                    std::vector<int> slot_numbers, int handle, int server) {
    assert(runling_data.size() == slot_numbers.size());
    std::vector<int> slots_data(8, 0);
    std::string s;
    s.reserve(2048); // selected to be ~2x as large as my bank
    // collect data we will need to compute signature
    std::vector<std::vector<std::vector<std::string>>> bank_tags;
    // header code
    s += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    s += "<Bank version=\"1\">\n";
    
    // ** Start of section: unit **
    s += "\t<Section name=\"unit\">\n";
    std::vector<std::vector<std::string>> unit_section;
    std::vector<std::string> unit_section_title;
    unit_section_title.push_back("unit");
    unit_section.push_back(unit_section_title);
    // save runling data
    for (unsigned int i = 0; i < runling_data.size(); i++) {
        int slot_number = slot_numbers[i];
        slots_data[slot_number-1] = 1;
        s += "\t\t<Key name=\"0" + std::to_string(slot_number) + "\">\n";
        std::string runling_code = bankgen::encode_runling_data(runling_data[i]);
        s += "\t\t\t<Value string=\"" + runling_code + "\"/>\n";
        s += "\t\t</Key>\n";
        unit_section.push_back(bankgen::make_key_vec("0" + std::to_string(slot_number), "string", runling_code));
    }
    // save slots info
    s += "\t\t<Key name=\"info\">\n";
    std::string slots_code = bankgen::encode_slots_data(slots_data);
    s += "\t\t\t<Value string=\"" + slots_code + "\"/>\n";
    s += "\t\t</Key>\n";
    unit_section.push_back(bankgen::make_key_vec("info", "string", slots_code));
    // ** End of section: unit **
    s += "\t</Section>\n";
    bank_tags.push_back(unit_section);
    
    // ** Start of section: account **
    s += "\t<Section name=\"account\">\n";
    std::vector<std::vector<std::string>> account_section;
    std::vector<std::string> account_section_title;
    account_section_title.push_back("account");
    account_section.push_back(account_section_title);
    // save account data
    s += "\t\t<Key name=\"info\">\n";
    std::string account_code = bankgen::encode_account_data(account_data);
    s += "\t\t\t<Value string=\"" + account_code + "\"/>\n";
    s += "\t\t</Key>\n";
    account_section.push_back(bankgen::make_key_vec("info", "string", account_code));
    // save checksums
    s += "\t\t<Key name=\"camera\">\n";
    int account_checksum = bankgen::compute_account_checksum(account_data);
    int character_checksum = bankgen::compute_character_checksum(runling_data, handle);
    std::string checksum_code = bankgen::encode_checksums(account_checksum, character_checksum);
    s += "\t\t\t<Value string=\"" + checksum_code + "\"/>\n";
    s += "\t\t</Key>\n";
    account_section.push_back(make_key_vec("camera", "string", checksum_code));
    // ** End of section: account **
    s += "\t</Section>\n";
    bank_tags.push_back(account_section);
    
    // ** Signature **
    std::string signature = bankgen::compute_signature(handle, server, bank_tags);
    s += "\t<Signature value=\"" + signature + "\"/>\n";
    
    // End of bank file
    s += "</Bank>\n";
    return s;
}

void bankgen::write_to_file(std::string filename, std::string s) {
    std::ofstream ofs(filename, std::ofstream::out);
    ofs << s;
    ofs.close();
    std::cout << "Bank generated and saved to " << filename << std::endl;
}








/**

void print_bank_tags(vector<vector<vector<string>>> bank_tags) {
    cout << endl;
    for (vector<vector<string>> section_tags: bank_tags) {
        cout << "--Section--" << endl;
        for (vector<string> v: section_tags) {
            cout << svtos(v) << endl;
        }
    }
    cout << endl;
}

void print_default_account_data() {
    cout << "Default account data:" << endl;
    for (uint i = 0; i < rlr4::default_account_data.size(); i++) {
        cout << rlr4::account_data_labels[i] << ": ";
        cout << rlr4::default_account_data[i] << " / ";
        cout << rlr4::account_data_maxvals[i] << endl;
    }
    cout << endl;
    return;
}

void parse_lines(ifstream &fin, unsigned int num_lines, vector<int> &results) {
    string comment_delim("//");
    string line;
    unsigned int i = 0;
    results = vector<int>(num_lines, 0);
    while ((i < num_lines) && getline(fin, line)) {
        //if (fin.eof()) {
        //    break;
        //}
        if (!string_startswith(line, comment_delim)) {
            // add an extra check here: we only read lines with ":" in them
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string to_parse = line.substr(colon_pos+1, line.length()-colon_pos-1);
                results[i] = stoi(to_parse);
                i++;
            }
        }
    }
    if (i == 0) {
        results.clear();
    }
    return;
}

// alphabet + digits + punctuation + space
int legal_char(char c) {
    return ((' ' <= c) && (c <= '~'));
}

string parse_line_string(ifstream &fin) {
    string comment_delim("//");
    string line;
    while (getline(fin, line)) {
        if (!string_startswith(line, comment_delim)) {
            // add an extra check here: we only read lines with ":" in them
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                // string start - find first non-space after ":"
                unsigned int i = colon_pos;
                while (isspace(line[++i]));
                // string end - find first invalid character
                unsigned int j = i;
                while (legal_char(line[++j]));
                return line.substr(i, j-i);
            }
        }
    }
    return "";
}

int handle;
vector<int> account_data; 
vector<vector<int>> runling_data;
int main() {
    const string input_filename = "input_data.txt";
    // parse input from file
    ifstream fin(input_filename, ifstream::in);
    vector<int> results;
    cout << "Parsing input data from " << input_filename << "..."<< endl << endl;
    // output filename
    string output_filename = parse_line_string(fin);
    // handle
    parse_lines(fin, 1, results);
    handle = results[0];
    // account data
    parse_lines(fin, rlr4::account_data_labels.size(), results);
    account_data = results;
    // runling data
    vector<int> slot_numbers;
    for (int i = 0; i < 8; i ++) {
        // input file contains an extra value: Slot
        // input file does not contain the value: Remaining Points
        parse_lines(fin, rlr4::runling_data_labels.size(), results);
        if (results.empty()) break;
        // get slot number, then remove from results
        slot_numbers.push_back(results[0]);
        results.erase(results.begin());
        // compute remaining points
        int available_points = results[7]*4;
        int used_points = calc_points_used(results[2], results[3], results[4], results[5], results[6]);
        int delta = available_points - used_points;
        if (delta < 0) {
            cout << "Warning: used " << -delta;
            cout << " more points than available in slot " << slot_numbers.back() << endl;
            delta = 0;
        }
        results.push_back(delta); // add remaining points to our results
        runling_data.push_back(results);
    }
    fin.close();
    
    cout << "Handle: " << handle << endl << endl;
    cout << "Account data:" << endl;
    for (unsigned int i = 0; i < account_data.size(); i++) {
        cout << rlr4::account_data_labels[i] << ": ";
        cout << rlr4::default_account_data[i] << endl;
    }
    cout << endl;
    
    assert(runling_data.size() == slot_numbers.size());
    cout << runling_data.size() << " runling(s) found" << endl;
    for (unsigned int i = 0; i < runling_data.size(); i++) {
        cout << "Slot " << slot_numbers[i] << ": " << vtos(runling_data[i]) << endl;
    }
    cout << endl;
    
    
    
    // write to file
    std::ofstream fout(output_filename, ofstream::out);
    fout << generate_bank(account_data, runling_data, slot_numbers, handle);
    fout.close();
    cout << "Bank generated and saved to " << output_filename << endl;
    
    
    return 0;
}
**/
