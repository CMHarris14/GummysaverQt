#ifndef GAMELIST_H
#define GAMELIST_H

#include "../util/exceptions.cpp"
#include "gamehandler.h"

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
    std::optional<GameHandler> game_by_title(QString title);
    std::optional<GameHandler> game_at(int index);

    void save_database();
};

#endif // GAMELIST_H
