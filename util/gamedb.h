#ifndef GAMEDB_H
#define GAMEDB_H

#include <QString>
#include <SQLiteCpp/SQLiteCpp.h>

class gamedb {
  public:
    gamedb();
    bool initialized();

    void add_game(const std::string &name);
    void add_game(const std::string &name, const std::string &path);

    void update_game_image(int gameId, const std::string &path);

  private:
    const char *DB_NAME = "GameData.db3";
    SQLite::Database db;
    bool isInitialized;

    void create_tables() {
        // Games
        db.exec(R"(
        CREATE TABLE IF NOT EXISTS Games (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            ImgPath TEXT);
        )");

        // Paths
        db.exec(R"(
        CREATE TABLE IF NOT EXISTS Paths (
            id INTEGER,
            Path TEXT NOT NULL,
            FOREIGN KEY (id) REFERENCES Games(id)
            );
        )");

        // Backups
        db.exec(R"(
        CREATE TABLE IF NOT EXISTS Backups (
            id INTEGER,
            Name TEXT NOT NULL,
            Date DATETIME NOT NULL,
            FOREIGN KEY (id) REFERENCES Games(id)
            );
        )");
    }
};

#endif // GAMEDB_H
