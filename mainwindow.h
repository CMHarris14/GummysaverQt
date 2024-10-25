#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "games/gamelist.h"
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
    int working_game_index;

  private slots:
    void on_button_addGame_clicked();

    void on_list_games_currentItemChanged(QListWidgetItem *current,
                                          QListWidgetItem *previous);

    void on_button_addPath_clicked();

  private:
    Ui::MainWindow *ui;
    void refresh_game_list();
    void refresh_path_list();
    void refresh_backup_list();
};
#endif // MAINWINDOW_H
