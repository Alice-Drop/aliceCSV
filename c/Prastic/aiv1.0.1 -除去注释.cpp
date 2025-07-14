#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> parseLine(const std::string& line, const std::string& delimiter = ",") {
    std::vector<std::string> output_list;
    bool quoting = false;
    std::string cache;
    std::istringstream stream(line);

    std::string item;
    while (std::getline(stream, item, delimiter[0])) {
        if (quoting) {
            if (item.back() == '"') {
                cache += item.substr(0, item.size() - 1);
                size_t pos = cache.find("\"\"");
                if (pos != std::string::npos && pos < cache.size() - 1) {
                    cache.replace(pos, 2, "\"");
                }
                output_list.push_back(cache);
                cache.clear();
                quoting = false;
            } else {
                cache += item + delimiter;
            }
        } else {
            if (item.empty()) {
                output_list.push_back("");
            } else if (item.front() == '"' && item.back() == '"') {
                item = item.substr(1, item.size() - 2);
                item = item.replace(item.find("\"\""), 2, "\"");
                output_list.push_back(item);
            } else if (item.front() == '"') {
                quoting = true;
                cache = item;
            } else {
                output_list.push_back(item);
            }
        }
    }

    if (quoting) {
        output_list.push_back(cache);
    }

    return output_list;
}

std::vector<std::vector<std::string>> parseCSV(const std::string& csvText, const std::string& delimiter = ",") {
    std::vector<std::vector<std::string>> data;
    std::istringstream stream(csvText);
    std::string line;

    while (std::getline(stream, line)) {
        data.push_back(parseLine(line, delimiter));
    }

    return data;
}

std::vector<std::vector<std::string>> fixLineLength(const std::vector<std::vector<std::string>>& csv_sheet) {
    size_t maxLength = 0;
    for (const auto& row : csv_sheet) {
        maxLength = std::max(maxLength, row.size());
    }

    std::vector<std::vector<std::string>> fixed_csv_sheet = csv_sheet;
    for (auto& row : fixed_csv_sheet) {
        while (row.size() < maxLength) {
            row.push_back("");
        }
    }

    return fixed_csv_sheet;
}

std::string writeCSV(const std::vector<std::vector<std::string>>& table, const std::string& delimiter = ",", const std::string& sheet_encoding = "UTF-8", const std::string& line_break = "\n") {
    std::string csv_text;
    for (const auto& row : table) {
        for (size_t i = 0; i < row.size(); ++i) {
            const std::string& col = row[i];
            if (col.find('"') != std::string::npos || col.find(delimiter) != std::string::npos || col.find('\n') != std::string::npos) {
                csv_text += "\"";
                csv_text += col;
                csv_text += "\"";
            } else {
                csv_text += col;
            }
            if (i != row.size() - 1) {
                csv_text += delimiter;
            }
        }
        csv_text += line_break;
    }

    return csv_text;
}

void fixCSV(const std::string& path, const std::string& output_path = "output.csv",
            const std::string& origin_delimiter = ",", const std::string& target_delimiter = ",",
            const std::string& origin_encoding = "UTF-8", const std::string& target_encoding = "UTF-8",
            const std::string& target_line_break = "\n", bool fix_length = true) {
    std::ifstream my_file(path, std::ios::in | std::ios::binary);
    if (!my_file.is_open()) {
        std::cerr << "无法打开文件: " << path << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << my_file.rdbuf();
    std::string csv_ext = buffer.str();
    my_file.close();

    std::vector<std::vector<std::string>> table = parseCSV(csv_ext, origin_delimiter);

    if (fix_length) {
        table = fixLineLength(table);
    }

    std::ofstream output_file(output_path, std::ios::out | std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "无法打开文件: " << output_path << std::endl;
        return;
    }

    std::string csv_text = writeCSV(table, target_delimiter, target_encoding, target_line_break);
    output_file << csv_text;
    output_file.close();
}
