#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"
#include "iostream"
#include <QDir>
#include "choosefiledialog.h"
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QtNetwork>
#include <QEventLoop>
#include "htmlparser.h"
#include "mangainfodisplay.h"


QDir MainWindow::defaultDownloadDirectory = QDir(QDir::home().path()+"/Downloads/MangacalDownloads/");
QString MainWindow::domainLink = "https://mangasee123.com";
QJsonArray MainWindow::jarray;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!defaultDownloadDirectory.exists()){
        defaultDownloadDirectory.cdUp();
        defaultDownloadDirectory.mkdir("MangacalDownloads");
        defaultDownloadDirectory.cd("MangacalDownloads");
    }
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(manga_network_come(QNetworkReply *)));
    manager->get(QNetworkRequest(QUrl(domainLink + "/search/?name="+"asd")));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionchoose_download_location_triggered()
{
//    std::cout<<defaultDownloadDirectory.path().toStdString()<<" "<<defaultDownloadDirectory.exists()<<std::endl;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    defaultDownloadDirectory.path(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    defaultDownloadDirectory = QDir(dir);

}


void MainWindow::manga_network_come(QNetworkReply* reply){
//    std::cout<<"aaaaa"<<std::endl;
//    std::cout<<reply->readAll().toStdString()<<std::endl;
    std::string s = reply->readAll().toStdString();
    size_t t1 = s.find("vm.Directory = ") + 15;
    size_t t2 = s.find("vm.GetIntValue",t1)-1;
    s = s.substr(t1,t2-t1);
    t1 = s.find_last_of(';');
    s = s.substr(0,t1);

    QJsonDocument doc = QJsonDocument::fromJson(QString(s.c_str()).toUtf8());
    QJsonArray arr = doc.array();
    MainWindow::jarray = arr;

//    std::cout<<htmlParser::getMangas(reply->readAll().toStdString())->at(0).getName()<<std::endl;
    reply->close();
}
void MainWindow::on_pushButton_clicked()
{
    if(jarray.size() < 1){
        std::cout<<"not yet"<<std::endl;
        return;
    }
    QPlainTextEdit* te = ui->mangaName;
    QString searchText = te->toPlainText();
//    std::cout<<(domainLink + "/search/?name="+searchText).toStdString()<<std::endl;
    std::vector<mangaInfo*>* info = htmlParser::getMangas(*new QJsonArray(MainWindow::jarray),searchText.toStdString());
    QWidget* mangaScrollContent = new QWidget();
    QVBoxLayout * layout = new QVBoxLayout();
    mangaScrollContent->setLayout(layout);
    ui->scrollArea->setWidget(mangaScrollContent);
    for(int i = 14;i>-1;i--){
        mangaInfo* inf0 = info->at(i);
    mangaInfoDisplay* disp = new mangaInfoDisplay(mangaScrollContent,inf0);
    layout->insertWidget(0,disp);
    }

}



