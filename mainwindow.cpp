#include "mainwindow.h"
#include "ui_mainwindow.h"

QString curr_logged = "0";

std::vector <std::vector <QString>> getQuery(QString id){
    std::vector <std::vector <QString>> line;
    QSqlQuery query;
    query.exec("SELECT * FROM log"+id);
    while (query.next()) {
       std::vector<QString> temp;
        for (int i=0;i<5;i++){
            QString name = query.value(i).toString();
            temp.push_back(name);
            //qDebug() << name;
        }
        line.push_back(temp);
        }
    return line;
}

std::vector <QStringList> getProducts(QString idx){
    std::vector <QStringList> Products;
    Q_INIT_RESOURCE(source);
    QFile file(":/"+ curr_logged +"/"+ idx +".txt");
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        qDebug() << "Waring Cannon open file: " ;
    }
    QTextStream in(&file);
    while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList list = line.split(QLatin1Char(' '));
          //qDebug()<<list;
          Products.push_back(list);
       }
       file.close();

    return Products;
};
QString getLoginId(QString login, QString password){
    QSqlQuery query;
    query.exec("SELECT * FROM users WHERE login =\""+login+"\"");
       while (query.next())
       {
          if (query.value(2).toString()== password) return query.value(0).toString();
       }
    return "0";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::GoToProducts(){
    qDeleteAll(ui->scrollAreaWidgetContents->children());
}

void MainWindow::on_pushButton_2_clicked()
{
    QString ide = getLoginId(ui->lineEdit->text(),ui->lineEdit_2->text());
    curr_logged = ide;
    qDebug() <<ide;
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    QGridLayout * lay = new QGridLayout();
    std::vector<std::vector <QString>> logs = getQuery(ide);
    QString header[5] = {"lp","data","nr paragonu","id kasy","NIP"};
    for (int i=0;i<5;i++){
        QLabel *lab = new QLabel(header[i]);
        lab->setAlignment(Qt::AlignCenter);
        //lab->setFrameShape(QFrame::StyledPanel);
        lay->addWidget(lab,0,i);
    }
    QLabel *lab = new QLabel("Produkty");
    lab->setAlignment(Qt::AlignCenter);
    lay->addWidget(lab,0,5);

    for (int j=0;j<(int)logs.size();j++){
        QPushButton *but = new QPushButton("Produkty");
        //przycisk Produkty

        connect(but, &QPushButton::clicked, [j, this](){
            float suma=0;
            qDeleteAll(ui->scrollAreaWidgetContents->children());
            std::vector <QStringList> Products = getProducts(std::to_string(j+1).c_str());
            QGridLayout * lay = new QGridLayout();

            QString header[4] = {"nazwa","VAT","ilość","cena"};
            for (int i=0;i<4;i++){
                QLabel *lab = new QLabel(header[i]);
                lab->setAlignment(Qt::AlignCenter);
                //lab->setFrameShape(QFrame::StyledPanel);
                lay->addWidget(lab,0,i);
            }
            for (int j=0;j<(int)Products.size();j++){
                for(int i=0;i<4;i++){
                    QLabel *lab = new QLabel(Products[j][i]);
                    lab->setFrameShape(QFrame::StyledPanel);
                    lay->addWidget(lab,j+1,i);
                }
                suma += Products[j][3].toFloat();
            }

            QString txt="Suma PLN: ";
            qDebug()<<suma;
            txt += std::to_string(suma).c_str();
            QLabel *lab = new QLabel(txt);
            lab->setAlignment(Qt::AlignCenter);
            lay->addWidget(lab,(int)Products.size()+1,0);
            ui->scrollAreaWidgetContents->setLayout(lay);

        });


        for(int i=0;i<5;i++){
            QLabel *lab = new QLabel(logs[j][i]);
            lab->setFrameShape(QFrame::StyledPanel);
            lay->addWidget(lab,j+1,i);
        }
        lay->addWidget(but,j+1,5);
    }
    lay->setHorizontalSpacing(20);
    lay->setVerticalSpacing(20);
    ui->scrollAreaWidgetContents->setLayout(lay);

}


void MainWindow::on_pushButton_clicked()
{
    QString login = ui->lineEdit->text(), password = ui->lineEdit_2->text();
    QSqlQuery query;
    QString q="INSERT INTO users(login, password) values(\""+login+"\",\""+password+"\");";
    //qDebug()<<q;
    query.exec(q);
    query.exec("SELECT ids from users ORDER BY ids DESC LIMIT 1");
    QString id=0;
    while(query.next()){
         id = query.value(0).toString();
         //qDebug() <<id;
    }
    q = "CREATE TABLE log"+id+"(ids integer primary key, data tex, nr_prg text, id_kasy text, NIP text);";
    query.exec(q);
    //qDebug()<<q;

}

