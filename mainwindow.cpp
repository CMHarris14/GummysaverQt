#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_button_addGame_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Game", "Enter game name",
                                         QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->list_games->addItem(name);
    } else if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Name cannot be empty");
    }
}
