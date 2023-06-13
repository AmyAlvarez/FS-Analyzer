#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <queue>

namespace fs = std::filesystem;

void traverseFileSystem(const fs::path& path, std::unordered_map<std::string, std::size_t>& fileSizes,
                        std::unordered_map<std::size_t, std::string>& duplicates,
                        std::priority_queue<std::pair<std::size_t, std::string>>& largestFiles) {
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_regular_file(entry)) {
            // Calculate file size
            std::size_t fileSize = fs::file_size(entry);

            // Store file size
            fileSizes[entry.path().string()] = fileSize;

            // Check for duplicates
            if (duplicates.find(fileSize) != duplicates.end()) {
                duplicates[fileSize] += ", " + entry.path().string();
            } else {
                duplicates[fileSize] = entry.path().string();
            }

            // Update largest files
            largestFiles.push({ fileSize, entry.path().string() });
            if (largestFiles.size() > 10) {
                largestFiles.pop();
            }
        }
    }
}

int main() {
    fs::path pathToAnalyze = "C:/Users/amyba/OneDrive/Desktop/Projects/FS analyzer";

    std::unordered_map<std::string, std::size_t> fileSizes;
    std::unordered_map<std::size_t, std::string> duplicates;
    std::priority_queue<std::pair<std::size_t, std::string>> largestFiles;

    traverseFileSystem(pathToAnalyze, fileSizes, duplicates, largestFiles);

    // Calculate total file size
    std::size_t totalSize = 0;
    for (const auto& fileSize : fileSizes) {
        totalSize += fileSize.second;
    }
    std::cout << "Total file size: " << totalSize << " bytes\n";

    // Print duplicate files
    std::cout << "Duplicate files:\n";
    for (const auto& duplicate : duplicates) {
        if (duplicate.second.find(',') != std::string::npos) {
            std::cout << "Size: " << duplicate.first << " bytes\n";
            std::cout << "Files: " << duplicate.second << "\n";
        }
    }

    // Print largest files
    std::cout << "Largest files:\n";
    while (!largestFiles.empty()) {
        std::cout << "Size: " << largestFiles.top().first << " bytes\n";
        std::cout << "File: " << largestFiles.top().second << "\n";
        largestFiles.pop();
    }

    return 0;
}

