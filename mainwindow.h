#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "games/gamelist.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GameList game_list;
    QString working_game = "";

  private slots:
    void on_button_addGame_clicked();

    void on_list_games_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_button_addPath_clicked();

    void on_button_addFile_clicked();

    void on_button_deletePath_clicked();

    void on_button_addBackup_clicked();

    void on_button_deleteBackup_clicked();

    void on_button_renameBackup_clicked();

    void on_button_restoreBackup_clicked();

  private:
    Ui::MainWindow *ui;
    void refresh_game_list();
    void refresh_path_list();
    void refresh_backup_list();
    QVector<QWidget *> toggled_buttons;

    void build_button_list() {
        toggled_buttons.append(ui->button_addFile);
        toggled_buttons.append(ui->button_addPath);
        toggled_buttons.append(ui->button_deletePath);
        toggled_buttons.append(ui->button_addBackup);
        toggled_buttons.append(ui->button_deleteBackup);
        toggled_buttons.append(ui->button_renameBackup);
        toggled_buttons.append(ui->button_restoreBackup);
    }

    void disable_buttons() {
        for (QWidget *button : toggled_buttons) {
            button->setEnabled(false);
        }
    }

    void enable_buttons() {
        for (QWidget *button : toggled_buttons) {
            button->setEnabled(true);
        }
    }
};
#endif // MAINWINDOW_H
