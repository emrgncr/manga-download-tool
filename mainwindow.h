#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QtNetwork>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static QDir defaultDownloadDirectory;
    static QString domainLink;
    static QJsonArray jarray;

private slots:
    void on_actionchoose_download_location_triggered();

    void on_pushButton_clicked();

    void manga_network_come(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
