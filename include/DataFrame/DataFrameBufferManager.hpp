// include/DataFrameBufferManager.hpp
#ifndef DATAFRAMEBUFFERMANAGER_HPP
#define DATAFRAMEBUFFERMANAGER_HPP

#include "DataFrame.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <string>

class DataFrameBufferManager {
private:
    std::unordered_map<std::string, std::shared_ptr<DataFrame>> dataFrameBuffers;
    mutable std::mutex bufferMutex;

public:
    // Atualiza ou cria um novo buffer de DataFrame
    void updateBuffer(const std::string& bufferName, std::shared_ptr<DataFrame> newDataFrame) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        dataFrameBuffers[bufferName] = newDataFrame;
    }

    // Obtém um buffer de DataFrame se existir
    std::shared_ptr<DataFrame> getBuffer(const std::string& bufferName) const {
        std::lock_guard<std::mutex> lock(bufferMutex);
        auto it = dataFrameBuffers.find(bufferName);
        if (it != dataFrameBuffers.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Limpa um buffer de DataFrame
    void clearBuffer(const std::string& bufferName) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        dataFrameBuffers.erase(bufferName);
    }
};

#endif // DATAFRAMEBUFFERMANAGER_HPP
