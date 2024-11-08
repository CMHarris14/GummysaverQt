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
        throw(CustomException::DBError);
    }
}

bool gamedb::initialized() { return isInitialized; }

// Add a game with or without a path to a cover image
void gamedb::add_game(const std::string &name) {
    SQLite::Statement query(db, "INSERT INTO Games (Name, ImgPath) VALUES (?, ?)");
    query.bind(1, name);
    query.bind(2, SQLite::Null);
    query.exec();
}

void gamedb::add_game(const std::string &name, const std::string &path) {
    SQLite::Statement query(db, "INSERT INTO Games (Name, ImgPath) VALUES (?, ?)");
    query.bind(1, name);

    if (path.empty()) {
        query.bind(2, SQLite::Null);
    } else {
        query.bind(2, path);
    }
    query.exec();
}

void gamedb::add_path(const std::string &game, const std::string &path) {
    try {
        int game_id = get_id_by_name(game);
        if (game_id >= 0) {
            SQLite::Statement addPath(db, "INSERT INTO Paths (id, Path) VALUES (?, ?)");
            addPath.bind(1, game_id);
            addPath.bind(2, path);
            addPath.exec();
        }
    } catch (SQLite::Exception &e) {
        Logger::log(Logger::ERROR, e.what());
    }
}

void gamedb::delete_path(const std::string &game, const std::string &path) {
    try {
        int game_id = get_id_by_name(game);
        if (game_id >= 0) {
            SQLite::Statement removePath(db, "DELETE FROM Paths WHERE id = ? AND Path = ?");
            removePath.bind(1, game_id);
            removePath.bind(2, path);
            removePath.exec();
        }
    } catch (SQLite::Exception &e) {
        Logger::log(Logger::ERROR, e.what());
    }
}

// Completely remove a game and related data from the database
void gamedb::delete_game(const std::string &name) {
    try {
        int game_id = get_id_by_name(name);
        if (game_id >= 0) {
            SQLite::Transaction transaction(db);

            // Delete paths of associated game
            SQLite::Statement deletePaths(db, "DELETE FROM Paths WHERE id = ?");
            deletePaths.bind(1, game_id);
            deletePaths.exec();

            // Delete the game itself
            SQLite::Statement deleteGame(db, "DELETE FROM Games WHERE id = ?");
            deleteGame.bind(1, game_id);
            deleteGame.exec();

            transaction.commit();
        }

    } catch (const SQLite::Exception &e) {
        Logger::log(Logger::ERROR, e.what());
    }
}

int gamedb::get_id_by_name(const std::string &name) {
    SQLite::Statement query(db, "SELECT id FROM Games WHERE Name = ?");
    query.bind(1, name);

    if (query.executeStep()) {
        return query.getColumn(0).getInt();
    }
    return -1;
}

// Get all game names as a list of strings
QStringList gamedb::get_games() {
    QStringList games;

    try {
        SQLite::Statement query(db, "SELECT Name FROM Games");

        // Iterate through results
        while (query.executeStep()) {
            QString name = QString::fromStdString(query.getColumn(0).getString());
            games.append(name);
        }
    } catch (const SQLite::Exception &e) {
        std::ostringstream tmp;
        tmp << "Failed to execute query in get_games() : " << e.what();
        Logger::log(Logger::ERROR, tmp.str());
        throw(CustomException::DBError);
    }

    return games;
}

// Get all paths associated with the given game
QStringList gamedb::get_paths(const std::string &name) {
    QStringList paths;

    try {
        int game_id = get_id_by_name(name);
        if (game_id >= 0) {
            SQLite::Statement query(db, "SELECT Path FROM Paths WHERE id = ?");
            query.bind(1, game_id);

            while (query.executeStep()) {
                QString path = QString::fromStdString(query.getColumn(0).getString());
                paths.append(path);
            }
        }
    } catch (const SQLite::Exception &e) {
        std::ostringstream tmp;
        tmp << "Failed to execute query in get_paths() : " << e.what();
        Logger::log(Logger::ERROR, tmp.str());
        throw(CustomException::DBError);
    }

    return paths;
}

QString gamedb::get_image_path(const std::string &game) {
    try {
        SQLite::Statement query(db, "SELECT ImgPath FROM Games WHERE Name = ?");
        query.bind(1, game);
        if (query.executeStep()) {
            return QString::fromStdString(query.getColumn(0).getString());
        }
    } catch (const SQLite::Exception &e) {
        std::ostringstream tmp;
        tmp << "Failed to execute query in get_image_path() : " << e.what();
        Logger::log(Logger::ERROR, tmp.str());
        throw(CustomException::DBError);
    }
    return "";
}

// Update the cover art path for the given game
void gamedb::update_game_image(const std::string &game, const std::string &path) {
    try {
        int game_id = get_id_by_name(game);
        if (game_id >= 0) {
            SQLite::Statement query(db, "UPDATE Games SET ImgPath = ? WHERE id = ?");

            if (path.empty()) {
                throw(CustomException::NoFilePath);
            } else {
                query.bind(1, path);
            }

            query.bind(2, game_id);

            int affectedRows = query.exec();

            if (affectedRows > 0) {
                std::ostringstream tmp;
                tmp << "Image for game with id: " << game_id << "succesfully updated to '" << path << "'";
                Logger::log(Logger::INFO, tmp.str());
            } else {
                Logger::log(Logger::ERROR, "No game found with id: " + std::to_string(game_id));
            }
        }

    } catch (const SQLite::Exception &e) {
        std::ostringstream tmp;
        tmp << "Failed to update image path: " << e.what();
        Logger::log(Logger::ERROR, tmp.str());
    }
}
