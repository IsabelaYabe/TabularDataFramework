#include "utils/file_processor.hpp"
#include <fstream>
#include <sstream>
#include <vector>

FileProcessor::FileProcessor(Queue<std::string>& inputQueue, Queue<std::string>& outputQueue) : inputQueue_(inputQueue), outputQueue_(outputQueue) {
    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        workerThreads.emplace_back(&FileProcessor::processFiles, this);
    }
}

FileProcessor::~FileProcessor() {
    for (auto& worker : workerThreads) {
        worker.join();
    }
}

void FileProcessor::processFiles() {
    while (true) {  
        auto optFilepath = inputQueue_.dequeue(); // Dequeue returns std::optional<std::string>
        if (!optFilepath) {
            std::cout << "No item to process. Thread is waiting...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep briefly to reduce CPU usage
            continue; // Continue checking for new files, or you could exit based on a condition
        }
        
        std::string filepath = *optFilepath; // Safely extract the filepath from std::optional
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << *optFilepath << std::endl;
            continue;
        }
        // Mutex should be static or class-wide to properly synchronize multi-thread access
        static std::mutex fileMutex; 
        std::lock_guard<std::mutex> guard(fileMutex); // Ensure file is accessed one at a time across threads
        std::cout << "Processing file: " << filepath << std::endl;
        if (fileState_.find(filepath) == fileState_.end()) {
            file.seekg(0, std::ios::beg);
        } else {
            file.seekg(fileState_[filepath]);
        }

        std::string line;
        std::string content;  
        std::streampos lastPos = file.tellg();
        while (getline(file, line)) {
            // std::cout << "Processing line: " << line << std::endl;
            if (!file.eof()) { // Check if the line is complete 
                // std::cout << line << std::endl;
                content += line + "\n";  
                lastPos = file.tellg();  // Update last position after reading a complete line
            }
        }  

        fileState_[filepath] = lastPos; // Save position to start here next time  
        fileLastAccess_[filepath] = std::chrono::steady_clock::now();
        std::cout << "enqueuing content: \n" << std::endl;
        outputQueue_.enqueue(content); // Enqueue the content for further processing

        cleanupFileStates();  
    }  
}

void FileProcessor::cleanupFileStates() {
    auto now = std::chrono::steady_clock::now();
    for (auto it = fileLastAccess_.begin(); it != fileLastAccess_.end(); ) {
        if (now - it->second > std::chrono::minutes(5)) {
            fileState_.erase(it->first);
            it = fileLastAccess_.erase(it);
        } else {
            ++it;
        }
    }
}