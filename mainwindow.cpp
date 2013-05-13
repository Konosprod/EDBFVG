#include "mainwindow.h"
#include "ui_mainwindow.h"

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
//#ifndef WIN32
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//#endif
    ui->setupUi(this);
    setupDataBase();
    setupTable();
    setupComboBox();


    ui->mainToolBar->addAction(ui->actionAjouter_console);
    ui->mainToolBar->addAction(ui->actionEditer_ligne);
    ui->mainToolBar->addAction(ui->actionSupprimer_ligne);

    connect(ui->buttonAdd, SIGNAL(clicked()), this, SLOT(addGame()));
    connect(ui->actionAjouter_console, SIGNAL(triggered()), this, SLOT(addConsole()));
    connect(ui->actionSupprimer_ligne, SIGNAL(triggered()), this, SLOT(deleteGame()));
    connect(ui->actionEditer_ligne, SIGNAL(triggered()), this, SLOT(editGame()));
    connect(ui->actionExporter_en_csv, SIGNAL(triggered()), this, SLOT(exportCSV()));
    connect(ui->actionImporter_un_CSV, SIGNAL(triggered()), this, SLOT(importCSV()));
}

void mainWindow::setupDataBase()
{
    m_dataBase = QSqlDatabase::addDatabase("QSQLITE");
    m_dataBase.setDatabaseName("edbfvg.sqlite");

    if(!m_dataBase.open())
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir la base de donnée !");
    }
    else
    {
        QString query = "";
        query.append("CREATE TABLE IF NOT EXISTS Games"
                 "(id integer primary key,"
                 "console VARCHAR(60) NOT NULL,"
                 "titre VARCHAR(150) NOT NULL,"
                 "annee VARCHAR(10),"
                 "devs VARCHAR(60)"
                 ")");

        QSqlQuery q;
        q.exec(query);
        q.exec("CREATE TABLE IF NOT EXISTS Consoles"
               "(id integer primary key,"
               "console VARCHAR(60) NOT NULL)");
    }
}

void mainWindow::setupComboBox()
{
    QSqlTableModel* m = new QSqlTableModel();
    m->setTable("Consoles");
    m->removeColumn(0);
    m->select();

    QStringList list;

    for(int i = 0; i < m->rowCount(); i++)
    {
        list.append(m->record(i).value(0).toString());
    }
    list.removeDuplicates();
    list.sort();
    ui->comboConsole->clear();
    ui->comboConsole->addItems(list);
}

void mainWindow::setupTable()
{
    m_model = new QSqlTableModel(this, m_dataBase);
    m_model->setTable("Games");
    m_model->removeColumn(0);
    m_model->setHeaderData(0, Qt::Horizontal, "Console");
    m_model->setHeaderData(1, Qt::Horizontal, "Titre");
    m_model->setHeaderData(2, Qt::Horizontal, "Année");
    m_model->setHeaderData(3, Qt::Horizontal, "Développeurs");
    m_model->select();
    this->ui->tableGames->setModel(m_model);
    this->ui->tableGames->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableGames->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void mainWindow::addGame()
{
    if(ui->lineTitle->text() == "")
    {
        QMessageBox::information(this, "Erreur", "Le jeu doit au moins avoir un titre !");
    }
    else
    {
        QString console = ui->comboConsole->currentText();
        QString date = ui->dateEdit->text();
        QString devs = ui->liveDev->text();
        QString titre = ui->lineTitle->text();
        QSqlQuery q;
        q.exec("insert into Games(id, console, titre, annee, devs)"
               "values(NULL,'" + console + "','" + titre + "','" + date + "','" + devs + "')");
        ui->lineTitle->clear();
        ui->dateEdit->setDate(QDate::fromString("2000"));
        ui->liveDev->clear();
        ui->comboConsole->setCurrentIndex(0);
    }

    m_model->select();
    //ui->tableGames->setModel(m_model);
}

void mainWindow::deleteGame()
{
    if(ui->tableGames->currentIndex().isValid())
    {
        QString console = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 0)).toString();
        QString title = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 1)).toString();
        QString annee = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 2)).toString();
        QString devs = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 3)).toString();
        QSqlQuery q;
        QString query = "DELETE FROM Games WHERE console='" + console +"' AND titre='" + title + "' AND annee='"+ annee +
                "' AND devs='" + devs + "';";
        q.exec(query);
        m_model->removeRow(ui->tableGames->currentIndex().row());
        m_model->select();
    }

}

void mainWindow::editGame()
{
    if(ui->tableGames->currentIndex().isValid())
    {
        QString console = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 0)).toString();
        QString title = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 1)).toString();
        QString annee = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 2)).toString();
        QString devs = ui->tableGames->model()->data(ui->tableGames->model()->index(ui->tableGames->currentIndex().row(), 3)).toString();

        QSqlTableModel* m = new QSqlTableModel();
        m->setTable("Consoles");
        m->removeColumn(0);
        m->select();

        QStringList list;

        for(int i = 0; i < m->rowCount(); i++)
        {
            list.append(m->record(i).value(0).toString());
        }
        list.removeDuplicates();

        editDialog* d = new editDialog(list, this, console, title, annee, devs);

        if(d->exec() == QDialog::Accepted)
        {
            QSqlQuery q;

            QString query = "UPDATE Games "
                    "SET console='" + d->console() + "', "
                    "titre='" + d->titre() + "', "
                    "annee='" + d->annee() + "', "
                    "devs='" + d->devs() + "' "
                    "WHERE console='" + console +"' AND titre='" + title + "' AND annee='"+ annee +
                    "' AND devs='" + devs + "';";

            q.exec(query);

            m_model->select();
        }
    }
}

void mainWindow::importCSV()
{
    if(QMessageBox::question(this, "Attention !", "Ouvrir un CSV écrasera vos données, voulez-vous continuer ?", QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton) == QMessageBox::Yes)
    {
        QSqlQuery q;
        QString query = "DELETE FROM Games";
        q.exec(query);

        QString filename = QFileDialog::getOpenFileName(this, "Exporter sous", QDir::homePath(), "*.csv");

        QFile f(filename);
        if(f.open(QFile::ReadOnly | QFile::Text))
        {
            while(!f.atEnd())
            {

                QString temp = f.readLine();

                QStringList fields = temp.split(";");
                q.exec("insert into Games(id, console, titre, annee, devs)"
                        "values(NULL,'" + fields.at(0) + "','" + fields.at(1) + "','" + fields.at(2) + "','" + fields.at(3) + "')");
            }
        }

        f.close();

        m_model->setTable("Games");
        m_model->removeColumn(0);
        m_model->setHeaderData(0, Qt::Horizontal, "Console");
        m_model->setHeaderData(1, Qt::Horizontal, "Titre");
        m_model->setHeaderData(2, Qt::Horizontal, "Année");
        m_model->setHeaderData(3, Qt::Horizontal, "Développeurs");
        m_model->select();
    }
}

void mainWindow::exportCSV()
{
    QString filename = QFileDialog::getSaveFileName(this, "Exporter sous", QDir::homePath(), "*.csv");
    if(filename != "")
    {
        if(filename.lastIndexOf(".csv") != filename.size()-4)
        {
            filename.append(".csv");
        }

        QFile f(filename);
        if(f.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &f );
            QStringList strList;

            for(int r = 0; r < ui->tableGames->model()->rowCount(); ++r )
            {
                strList.clear();
                for(int c = 0; c < ui->tableGames->model()->columnCount(); ++c )
                {
                    strList << ui->tableGames->model()->index( r, c ).data().toString();
                }
                data << strList.join( ";" )+"\n";
            }
          }

        f.close();
        QMessageBox::information(this, "Enregistré !", "Le fichier a bie été enregistré !");
    }
}

void mainWindow::addConsole()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Ajouter une console", "Console :", QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty())
    {
        QSqlQuery q;
        q.exec("INSERT OR IGNORE into Consoles(id, console)"
               "values(NULL,'" + text + "')");
        setupComboBox();
    }
}

mainWindow::~mainWindow()
{
    delete ui;
}
