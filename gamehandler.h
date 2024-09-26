#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QDateTime>
#include <QFile>
#include <QPixMap>

struct Backup {
    Backup(QString name, QDateTime time) {
        this->name = name;
        this->time = time;
    }
    QString name;
    QDateTime time;
};

class GameHandler {
  public:
    GameHandler();

    QPixmap *boxart;
    QString boxart_path;
    QVector<QString> paths;
    QVector<Backup> backup_list;

    void update_boxart(QString imgpath);

    void add_path(QString path);
    void remove_path(int index);
    void add_backup(QString name);
    void remove_backup(int index);
};

#endif // GAMEHANDLER_H
