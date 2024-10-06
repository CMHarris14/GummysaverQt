#ifndef GAMELIST_H
#define GAMELIST_H

#include "../util/exceptions.cpp"
#include "gamehandler.h"
#include <iostream>

#include <SQLiteCpp/SQLiteCpp.h>

class GameList {
  private:
    QVector<GameHandler> game_list;

  public:
    GameList();
    // Add a new game to manage. Titles must be unique
    void add_game(QString title);
    // Returns the titles of every managed game
    QVector<QString> get_game_titles();

    void save_database() {
        try {
            SQLite::Database db("test.db3",
                                SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        } catch (SQLite::Exception e) {
            std::cout << e.what() << std::endl;
        }
    }
};

#endif // GAMELIST_H
