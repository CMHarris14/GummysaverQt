#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <QOperatingSystemVersion>
#include <QProcess>

class archiver {
  public:
    archiver();
    const QString backup_dir = ".\\backups\\";
    void make_backup(QString game, QString backup_name, const QStringList &paths);

  private:
    QString zipper;
    void create_metadata(const QStringList &paths);
};

#endif // ARCHIVER_H
