//
// Created by Admin on 2024/7/29.
//
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// 解析CSV格式的字符串行
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
                cache = cache.replace(cache.find("\"\""), 2, "\"");
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

// 解析CSV文本
std::vector<std::vector<std::string>> parseCSV(const std::string& csvText, const std::string& delimiter = ",") {
    std::vector<std::vector<std::string>> data;
    std::istringstream stream(csvText);
    std::string line;

    while (std::getline(stream, line)) {
        data.push_back(parseLine(line, delimiter));
    }

    return data;
}

// 修复行长度
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

// 将二维列表写入CSV文件
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

int main() {
    // 示例CSV文本
    std::string csvText = "a,b,\"c,d\",e\nf,g,h,i";
    std::string delimiter = ","; // CSV分隔符

    // 解析CSV文本
    auto csv_data = parseCSV(csvText, delimiter);

    // 修复行长度
    auto fixed_csv_data = fixLineLength(csv_data);

    // 将二维列表写入CSV文件
    std::string csv_text = writeCSV(fixed_csv_data, delimiter);

    // 输出CSV文本
    std::cout << csv_text;

    return 0;
}
