#ifndef REPOSITORYSAVER_HPP
#define REPOSITORYSAVER_HPP

#include "../DataFrame/DataFrame.h"
#include "../queue/queue.h"
#include <sqlite3.h>
#include <string>

class RepositorySaver {
protected:
    Queue<DataFrame>& inputQueue;
    sqlite3* db;
    std::string tableName;

    std::string buildInsertQuery(const DataFrame& df) const;

public:
    RepositorySaver(Queue<DataFrame>& queue, const std::string& dbPath, const std::string& table);
    virtual ~RepositorySaver();

    virtual void saveToDatabase() = 0;
};

#endif // REPOSITORYSAVER_HPP
