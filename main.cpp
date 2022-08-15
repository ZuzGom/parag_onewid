#include "mainwindow.h"

#include <QApplication>
#include <bits/stdc++.h>
using namespace std;


int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(source);
    std::string path =  __FILE__;
    path = path.substr(0,path.size()-8) ;
    QString path1 = path.c_str() ;





    path+= "paragony.db";

    QApplication a(argc, argv);
    DbManager d(path.c_str());
    QFile styleSheetFile(":/img/Toolery.qss");
    styleSheetFile.open(QFile :: ReadOnly);
    QString stylesheet=QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(stylesheet);

    /*
    for(int i=1;i<=5;i++){
        QString sti =to_string(i).c_str();
        QString path2= path1 +"1\\"+sti+".txt";
        //path1+=
        qDebug()<<path2;
        QFile file(path2);
        QSqlQuery query;
        QString q = "CREATE TABLE p_1_"+sti+"(ids integer primary key, nazwa text, vat text, ilosc text, cena text);";
        query.exec(q);
        qDebug()<<q;
        string nazwa, vat, ilosc, cena;

        if(!file.open(QIODevice::ReadOnly | QFile::Text)){
            qDebug() << "Waring Cannon open file: " ;
        }
        QTextStream in(&file);
        while (!in.atEnd())
           {
              QString line = in.readLine();
              QStringList list = line.split(QLatin1Char(' '));

              q="INSERT INTO p_1_"+sti+"(nazwa, vat,ilosc,cena) values(\""+list[0]+"\",\""+list[1]+"\",\""+list[2]+"\","+list[3]+");";
              query.exec(q);
              qDebug()<<q;

           }
           file.close();
        }

*/

    MainWindow w;
    w.show();
    return a.exec();
}
