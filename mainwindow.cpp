#include "mainwindow.h"
#include "ui_mainwindow.h"


QString curr_logged = "0";

std::vector <std::vector <QString>> getLogs(QString id){
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

std::vector <std::vector <QString>> getProducts(QString idx){
    std::vector <std::vector <QString>> Products;

    QSqlQuery query;
    query.exec("SELECT * FROM p_"+curr_logged+"_"+idx);
    while (query.next()) {
       std::vector<QString> temp;
        for (int i=1;i<5;i++){
            QString name = query.value(i).toString();
            temp.push_back(name);
            //qDebug() << name;
        }
        Products.push_back(temp);
        }
    return Products;
};
void MainWindow::logIn(){
    QSqlQuery query;
    QString login= this->login_line->text(), password = this->password_line->text();

    query.exec("SELECT * FROM users WHERE login =\""+login+"\"");
       while (query.next())
       {
          if (query.value(2).toString()== password){
              curr_logged = query.value(0).toString();
              break;
             }
          else curr_logged = "0";
       }

}


void MainWindow::LoginPanel(){

    curr_logged = "0";
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    ui->foot->setText("");
    this->login_line = new QLineEdit("admin");
    this->password_line = new QLineEdit("admin");
    QVBoxLayout *ver = new QVBoxLayout();
    QHBoxLayout *hor = new QHBoxLayout();
    QLabel *llab = new QLabel("login");
    QLabel *plab = new QLabel("hasło");
    QLabel *lab = new QLabel("Witamy w eParagonie. Zaloguj się lub zarejestruj.");
    lab->setFrameShape(QFrame::StyledPanel);

    ver->addWidget(lab);
    ver->addWidget(llab);
    ver->addWidget(this->login_line);
    ver->addWidget(plab);
    ver->addWidget(this->password_line);

    QPushButton *but1 = new QPushButton("zaloguj");
    connect(but1, &QPushButton::released, this, &MainWindow::on_zaloguj_clicked);
    QPushButton *but2 = new QPushButton("zarejestruj");
    connect(but2, &QPushButton::released, this, &MainWindow::on_zarejestruj_clicked);
    hor->addWidget(but1);
    hor->addWidget(but2);

    ver->addLayout(hor);
    ui->scrollAreaWidgetContents->setLayout(ver);

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Q_INIT_RESOURCE(source);

    QPixmap pixmap(":/img/back.png");
    QIcon ButtonIcon(pixmap);
    ui->pushButton_3->setIcon(ButtonIcon);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::LoginPanel);
    this->LoginPanel();
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

void MainWindow::on_zaloguj_clicked(){
    this->logIn();
    this->LogsPanel();
}

void MainWindow::LogsPanel()
{
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    ui->foot->setText("");
    QGridLayout * lay = new QGridLayout();
    std::vector<std::vector <QString>> logs = getLogs(curr_logged);
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
            std::vector <std::vector <QString>> Products = getProducts(std::to_string(j+1).c_str());
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
            txt += QString::number(suma,'f',2);
            ui->foot->setText(txt);
            ui->scrollAreaWidgetContents->setLayout(lay);

        });
        //koniec przycisku Produkty

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

void MainWindow::on_zarejestruj_clicked()
{
    QString login= this->login_line->text(), password = this->password_line->text();
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
    q = "CREATE TABLE log"+id+"(ids integer primary key, data text, nr_prg text, id_kasy text, NIP text);";
    query.exec(q);
    //qDebug()<<q;

}


void MainWindow::on_pushButton_3_clicked()
{
    if (curr_logged!="0") this->LogsPanel();
}

