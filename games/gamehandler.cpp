#include "gamehandler.h"

GameHandler::GameHandler() { /*boxart = new QPixmap(boxart_path);*/ }

void GameHandler::update_boxart(QString imgpath) {
    QFile image(imgpath);

    if (image.exists()) {
        boxart = new QPixmap(imgpath);
    } else {
        // TODO: Get image from pcgamingwiki API
    }
}

void GameHandler::add_path(QString path) { paths.append(path); }
void GameHandler::remove_path(int index) { paths.removeAt(index); }
QVector<QString> GameHandler::get_paths() { return paths; }

void GameHandler::add_backup(QString name) {
    if (!name.isEmpty()) {
        Backup tmp = Backup(name, QDateTime::currentDateTime());
        backup_list.append(tmp);
    } else {
        throw std::invalid_argument("Name cannot be empty.");
    }
}

void GameHandler::remove_backup(int index) { backup_list.removeAt(index); }

QVector<Backup> GameHandler::get_backups() { return backup_list; }
Backup &GameHandler::backup_at(int index) { return backup_list[index]; }
