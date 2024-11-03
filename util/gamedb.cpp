#include "gamedb.h"
#include "exceptions.cpp"
#include "logger.h"
#include <iostream>
#include <sstream>

gamedb::gamedb() : db(DB_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), isInitialized(false) {
    try {
        create_tables();
        isInitialized = true;
    } catch (const SQLite::Exception &e) {
        Logger::log(Logger::ERROR, e.what());
    }
}

bool gamedb::initialized() { return isInitialized; }

// Add a game with or without a path to a cover image
void gamedb::add_game(const std::string &name) {
    SQLite::Statement query(db, "INSERT INTO Games (Name, ImgPath) VALUES (?, ?)");
    query.bind(1, name);
    query.bind(2, SQLite::Null);
}

void gamedb::add_game(const std::string &name, const std::string &path) {
    SQLite::Statement query(db, "INSERT INTO Games (Name, ImgPath) VALUES (?, ?)");
    query.bind(1, name);

    if (path.empty()) {
        query.bind(2, SQLite::Null);
    } else {
        query.bind(2, path);
    }
}

void gamedb::update_game_image(int gameId, const std::string &path) {
    try {
        SQLite::Statement query(db, "UPDATE Games SET ImgPath = ? WHERE id = ?");

        if (path.empty()) {
            throw(CustomException::NoFilePath);
        } else {
            query.bind(1, path);
        }

        query.bind(2, gameId);

        int affectedRows = query.exec();

        if (affectedRows > 0) {
            std::ostringstream oss;
            oss << "Image for game with id: " << gameId << "succesfully updated to '" << path << "'";
            Logger::log(Logger::INFO, oss.str());
        } else {
            Logger::log(Logger::ERROR, "No game found with id: " + std::to_string(gameId));
        }
    } catch (const SQLite::Exception &e) {
        std::ostringstream oss;
        oss << "Failed to update image path: " << e.what();
        Logger::log(Logger::ERROR, oss.str());
    }
}
