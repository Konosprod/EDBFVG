#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include "editdialog.h"

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();
protected:
    void setupDataBase();
    void setupTable();
    void setupComboBox();

public slots:
    void addGame();
    void addConsole();
    void deleteGame();
    void editGame();
    void exportCSV();
    void importCSV();

private:
    Ui::mainWindow *ui;
    QSqlTableModel* m_model;
    QSqlDatabase m_dataBase;
};

#endif // MAINWINDOW_H
