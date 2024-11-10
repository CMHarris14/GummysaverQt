#ifndef BACKUPRESTORE_H
#define BACKUPRESTORE_H

#include <QDialog>

namespace Ui {
class BackupRestore;
}

class BackupRestore : public QDialog {
    Q_OBJECT

  public:
    explicit BackupRestore(QWidget *parent = nullptr, QList<QPair<QString, QString>> *backup_files = nullptr);
    ~BackupRestore();

  private:
    Ui::BackupRestore *ui;
    void set_backup_info(QList<QPair<QString, QString>> *backup_files);
};

#endif // BACKUPRESTORE_H
