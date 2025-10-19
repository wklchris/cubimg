// g++ -std=c++20 doc.cpp -o doc
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>


struct AlgoData {
    // Cube algo sets: "PLL", "OLL", etc.
    std::string sets;
    // Cube algo name: "PLL-Aa", "OLL-1", etc.
    std::string name;
    // Cube algo category, "P-shape", etc.
    std::string category;
    // Cuba algo steps
    std::string algorithm;
    // Cube subcategory, especially for F2L slots. "RF", etc.
    std::string subcategory;
};

std::vector<AlgoData> parseCSV(const std::string& filename) {
    std::vector<AlgoData> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return data;
    }
    
    std::string line;
    bool firstLine = true;
    
    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        
        if (line.empty()) continue;
        
        AlgoData item;
        size_t start = 0;
        size_t end = line.find(',');
        
        item.sets = line.substr(start, end - start);
        
        start = end + 1;
        end = line.find(',', start);
        item.name = line.substr(start, end - start);
        
        start = end + 1;
        end = line.find(',', start);
        item.category = line.substr(start, end - start);
        
        start = end + 1;
        end = line.find(',', start);
        item.algorithm = line.substr(start, end - start);
        
        start = end + 1;
        if (start < line.length()) {
            item.subcategory = line.substr(start);
        }
        data.push_back(item);
    }
    return data;
}

std::string escapeHTML(const std::string& text) {
    std::string result;
    for (char c : text) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string escapeHTMLElementID(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for (char c : text) {
        switch (c) {
            // Replace all escape chars to hyphen
            case '&':
            case '<':
            case '>':
            case '"':
            case '\'':
            case ' ':  // Space to hyphen
                result += '-';
                break;
            default:
                result += c;
                break;
        }
    }
    return result;
}

void generateHTMLForSet(
    const std::vector<AlgoData>& data_,
    const std::string& image_ext,
    const std::string& set_name,
    const std::set<std::string>& all_sets
) {
    std::vector<AlgoData> setData;
    for (const auto& item : data_) {
        if (item.sets == set_name) {
            setData.push_back(item);
        }
    }
    
    if (setData.empty()) return;
    
    std::string html_content;
    html_content += "<!DOCTYPE html>\n";
    html_content += "<html>\n";
    html_content += "<head>\n";
    html_content += "<meta charset=\"utf-8\">";
    html_content += std::format("<title>{}</title>\n", escapeHTML(set_name));
    html_content += "<link rel=\"stylesheet\" type=\"text/css\" href=\"doc.css\">";
    html_content += "</head>\n";
    html_content += "<body>\n";
    
    std::vector<std::string> categories;  // Use vector to preserve insertion order
    for (const auto& item : setData) {
        std::string _category = item.category;
        if (std::ranges::find(categories, _category) == categories.end()) {
            categories.push_back(_category);
        }
    }
    
    bool hasSubcategory = false;
    for (const auto& item : setData) {
        if (!item.subcategory.empty()) {
            hasSubcategory = true;
            break;
        }
    }

    // Set navigation sidebar 
    html_content += "<nav class=\"sidebar\">\n";
    html_content += "<div class=\"sidebar-builder\">Plot with "
        "<img class=\"repo-icon\" src=\"img/icon.svg\">"
        "<a href=\"https://github.com/wklchris/cubimg\">CubImg</a>"
        "</div>\n<hr>\n";
    html_content += "<p class=\"sidebar-title\">Contents</p>\n";
    html_content += "<ul class=\"sidebar-list\">\n";
    for (const auto& set_ : all_sets) {
        if (set_ != set_name) {
            std::string set_escaped = escapeHTML(set_);
            html_content += std::format("<li><a href=\"{}.html\">{}</a></li>\n", 
                set_escaped, set_escaped
            );
        } else {
            // Current doc name
            html_content += std::format("<li class=\"current-doc\">{}</li>\n", escapeHTML(set_name));
            // Level-2 nav list for current doc
            html_content += "<ul class=\"current-doc-list\">\n";
            for (const auto& category : categories) {
                html_content += std::format("<li><a href=\"#{}\">{}</a></li>\n", 
                    escapeHTMLElementID(category), escapeHTML(category)
                );
            }
            html_content += "</ul>\n";
        }
    }
    html_content += "</ul>\n</nav>\n";
    html_content += "</nav>\n";
    
    // Set heading and paragraph
    html_content += "<section class=\"main\">\n";
    html_content += std::format("<h1>{}</h1>\n", escapeHTML(set_name));

    // Set table columns
    html_content += "<table class=\"algo-table\">\n";
    int colCount = hasSubcategory ? 4 : 3;
    html_content += "<colgroup>\n"
        "<col class=\"algo-image\">\n"
        "<col class=\"algo-name\">\n";
    if (hasSubcategory) {
        html_content += "<col class=\"algo-subcategory\">\n";
    }
    html_content += "<col class=\"algo\">\n</colgroup>\n";
    
    // Set table section head
    for (const auto& category : categories) {
        html_content += std::format(
            "<tr class=\"category-head\">"
            "<th id=\"{}\" colspan=\"{}\">{}</th>"
            "</tr>\n",
            escapeHTMLElementID(category), colCount, escapeHTML(category)
        );
        
        for (const auto& item : setData) {
            std::string item_name_escaped = escapeHTML(item.name);
            if (item.category == category) {
                html_content += "<tr>\n";
                // Image
                html_content += std::format(
                    "<td class=\"algo-image\"><img class=\"algo\" src=\"algo/{}.{}\" alt=\"{}\"></td>\n", 
                    item_name_escaped, image_ext, item_name_escaped
                );
                // Name
                html_content += std::format("<td class=\"algo-name\">{}</td>\n", item_name_escaped);
                // Subcategory
                if (hasSubcategory) {
                    html_content += std::format("<td class=\"algo-subcategory\">{}</td>\n", escapeHTML(item.subcategory));
                }
                // Algorithm
                html_content += std::format(
                    "<td class=\"algo\"><code class=\"algo\">{}</code></td>\n",
                    escapeHTML(item.algorithm)
                );
                html_content += "</tr>\n";
            }
        }
    }
    html_content += "</table>\n";
    html_content += "</section>\n";
    html_content += "</body>\n</html>\n";
    
    std::string filename = std::format("{}.html", set_name);
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << html_content;
        std::cout << "Generated: " << filename << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // The image extension used to load cube images in the HTML.
    std::string image_extension;
    image_extension = (argc > 1) ? argv[1] : "png";

    auto data = parseCSV("algo.csv");
    
    if (data.empty()) {
        std::cout << "Could not open file: algo.csv" << std::endl;
        return 1;
    }
    
    std::set<std::string> all_sets;
    for (const auto& item : data) {
        all_sets.insert(item.sets);
    }
    
    for (const auto& set_name : all_sets) {
        generateHTMLForSet(data, image_extension, set_name, all_sets);
    }
    
    std::cout << std::format(
        "Doc HTML generated (with {} images).", image_extension
    ) << std::endl;
    return 0;
}