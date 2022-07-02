#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout * lay = new QVBoxLayout();
    for (int i=0;i<10;i++){
        QHBoxLayout *hor = new QHBoxLayout();
        QPushButton *but = new QPushButton("nioml");
        QLabel *lab = new QLabel("oto on: ");
        hor->addWidget(lab);
        hor->addWidget(but);
        lay->addLayout(hor);

    }
    lay->setSpacing(13);
    ui->scrollAreaWidgetContents->setLayout(lay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

DbManager::DbManager(const QString& path)
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database failed";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

