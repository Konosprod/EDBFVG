#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class editDialog;
}

class editDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit editDialog(QStringList list, QWidget *parent = 0, QString console = "" , QString titre = "", QString annee = "", QString devs = "");
    QString console();
    QString titre();
    QString annee();
    QString devs();
    ~editDialog();
    
private:
    Ui::editDialog *ui;
};

#endif // EDITDIALOG_H
