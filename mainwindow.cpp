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


QDir MainWindow::defaultDownloadDirectory = QDir(QDir::home().path()+"/Downloads/MangacalDownloads/");
QString MainWindow::domainLink = "https://mangasee123.com";


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


void MainWindow::on_pushButton_clicked()
{
    QPlainTextEdit* te = ui->mangaName;
    QString searchText = te->toPlainText();
    std::cout<<(domainLink + "/search/?name="+searchText).toStdString()<<std::endl;
    QNetworkAccessManager manager;
    manager.get(QNetworkRequest(QUrl(domainLink + "/search/?name="+searchText)));

}
