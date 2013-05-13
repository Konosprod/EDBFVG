#include "editdialog.h"
#include "ui_editdialog.h"

editDialog::editDialog(QStringList consoles, QWidget *parent, QString console, QString titre, QString annee, QString devs) :
    QDialog(parent),
    ui(new Ui::editDialog)
{
    ui->setupUi(this);

    ui->comboConsole->clear();
    ui->comboConsole->addItems(consoles);
    ui->comboConsole->setCurrentIndex(ui->comboConsole->findText(console));

    ui->lineTitle->setText(titre);

    ui->liveDev->setText(devs);

    ui->dateEdit->setDateTime(QDateTime::fromString(annee));

}

editDialog::~editDialog()
{
    delete ui;
}

QString editDialog::console()
{
    return ui->comboConsole->currentText();
}

QString editDialog::titre()
{
    return ui->lineTitle->text();
}

QString editDialog::annee()
{
    return ui->dateEdit->text();
}

QString editDialog::devs()
{
    return ui->liveDev->text();
}
