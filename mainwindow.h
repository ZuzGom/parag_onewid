#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QtSql>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <vector>
#include <QPixmap>
#include <QIcon>

//#include <QMessageBox>
class DbManager
{
public:
    DbManager (const QString& path);
private:
    QSqlDatabase m_db;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void LoginPanel();
    void LogsPanel();
    void logIn();
    QLineEdit *login_line;
    QLineEdit *password_line;
    void on_zaloguj_clicked();
    void on_zarejestruj_clicked();

private slots:
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
