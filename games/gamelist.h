#ifndef GAMELIST_H
#define GAMELIST_H

#include "../util/exceptions.cpp"
#include "../util/gamedb.h"
#include <QDateTime>
// #include "gamehandler.h"

#include <SQLiteCpp/SQLiteCpp.h>

struct Backup {
    Backup(QString name, QDateTime time) {
        this->name = name;
        this->time = time;
    }
    QString name;
    QDateTime time;
};

class GameList {
  private:
    gamedb database;

  public:
    GameList();

    // Add a new game to manage. Titles must be unique
    void add_game(QString &title);
    // Delete game and all related data
    void delete_game(QString &title);

    void add_path(QString &game, QString &path);
    void delete_path(const QString &game, const QString &path);
    QVector<QString> get_paths(QString &game);

    QString get_image_path(QString &game);
    void update_game_image(QString &game, QString &path);
    // Returns the titles of every managed game
    QVector<QString> get_game_titles();
};

#endif // GAMELIST_H
