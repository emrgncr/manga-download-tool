#include "downloadpopup.h"
#include "ui_downloadpopup.h"
#include "mangainfo.h"
#include "sstream"
#include <math.h>
#include "QtNetwork"
#include "mainwindow.h"
#include "QTimer"
#include "hpdf.h"

QJsonArray DownloadPopup::arr;
int DownloadPopup::maxChapters = 100;

DownloadPopup::DownloadPopup(QWidget *parent, mangaInfo* info) :
    QDialog(parent),
    ui(new Ui::DownloadPopup)
{
    ui->setupUi(this);
    this->info = info;
    std::ostringstream ss1;
    ss1.precision(3);
    ss1 << info->getChapterCount();
    std::string s = ss1.str();

    ui->title->setText(QString::fromStdString(
                           info->getName() + " / " + s + " chapters"
                           ));
    ui->minspin->setMinimum(1);
    ui->maxspin->setMinimum(2);
    ui->maxspin->setMaximum((int)std::ceil(info->getChapterCount()));
    ui->minspin->setMaximum(1);
    ui->minspin->setValue(1);
    ui->maxspin->setValue((int)std::ceil(info->getChapterCount()));


    setAttribute(Qt::WA_DeleteOnClose);

}

DownloadPopup::~DownloadPopup()
{
    delete ui;
}

mangaInfo *DownloadPopup::getInfo() const
{
    return info;
}

void DownloadPopup::setInfo(mangaInfo *newInfo)
{
    info = newInfo;
}

std::string DownloadPopup::numToChapter(int x)
{
    std::string s = std::to_string(x);
    std::string ret = "000";
    for(size_t i = 0;i<3;i++){
        if(i >= s.length()) continue;
        ret[2-i] = s[s.length() - i -1];
    }
    return ret;
}

std::string DownloadPopup::numToChapter2(int x)
{
    std::string s = std::to_string(x);
    std::string ret = "0000";
    for(size_t i = 0;i<4;i++){
        if(i >= s.length()) continue;
        ret[3-i] = s[s.length() - i -1];
    }
    return ret;
}

std::string DownloadPopup::chapterCodeToChapter(int x)
{
    //sample chapter code 10010, 10045
    //sample output 0001, 0004.5
    x -= 100000;
    std::string y = std::to_string(x);
    if(x%10 == 0) return numToChapter2(x/10);
    std::string ret = numToChapter2(x/10) + "." + std::to_string(x%10);
    return ret;
}
std::string DownloadPopup::chapterCodeToChapterSmol(int x)
{
    //sample chapter code 10010, 10045
    //sample output 1, 4.5
    x -= 100000;
    std::string y = std::to_string(x);
    if(x%10 == 0) return std::to_string(x/10);
    std::string ret = std::to_string(x/10) + "." + std::to_string(x%10);
    return ret;
}


std::string DownloadPopup::getCoreURL(std::string x)
{
    return "";
}

std::vector<int>* DownloadPopup::getChaptersArray(std::string x)
{
    return nullptr;
}

void DownloadPopup::enterMainLoop()
{
    if(arr.size() == 0){
        ui->textBrowser->append("DOWNLOAD COMPLATE");
        ui->progressBar->setValue(100);
        return;
    }
    QJsonValue f = arr.last();
    arr.removeLast();
    int chapterint = f.toObject().value("Chapter").toString().toInt();
    int chapterSmol = (chapterint - 100000)/10;
    if(chapterSmol > ui->maxspin->value()) {
        enterMainLoop();
        return;
    }
    if(chapterSmol < ui->minspin->value()){
        enterMainLoop();
        return;
    }
    ui->textBrowser->append("Downloading chapter: " + QString::fromStdString(chapterCodeToChapterSmol(chapterint)));
    std::cout<<"ChapterSmol= "<<chapterSmol<<std::endl;
//Go to chapter website
    QString url = MainWindow::domainLink + "/read-online/" + QString::fromStdString(info->getName())
            + "-chapter-" + QString::fromStdString(chapterCodeToChapterSmol(chapterint))+".html";

    auto man = new QNetworkAccessManager();
    connect(man,&QNetworkAccessManager::finished,this,[=](QNetworkReply* res){
        gotChapterPage(res,(chapterint - 100000));
    });
    man->get(QNetworkRequest(QUrl(url)));

}

void DownloadPopup::savePage(QNetworkReply *res, int chapter, int page,std::string seriesName,int maxPages,std::string base_url)
{
    std::cout<<chapter << " " <<page << " " <<maxPages<<std::endl;
    QImage img;
    img.loadFromData(res->readAll());
//    ui->label_3->setPixmap(QPixmap::fromImage(img).scaled(ui->label_3->width(),ui->label_3->height()));
        MainWindow::defaultDownloadDirectory.mkdir(QString::fromStdString(seriesName) + "-"+QString::fromStdString(numToChapter2(chapter)));
        MainWindow::defaultDownloadDirectory.cd(QString::fromStdString(seriesName) + "-"+QString::fromStdString(numToChapter2(chapter)));
        std::cout<<(MainWindow::defaultDownloadDirectory.path()+"/" +QString::fromStdString(seriesName) + "-"+QString::fromStdString(numToChapter2(chapter))+"-"
                +QString::fromStdString(numToChapter(page))+".png").toStdString()<<std::endl;
    bool success = img.save(MainWindow::defaultDownloadDirectory.path()+"/" +QString::fromStdString(seriesName) + "-"+QString::fromStdString(numToChapter2(chapter))+"-"
                            +QString::fromStdString(numToChapter(page))+".png");

    std::cout<<"SUCCESS: "<<success<<std::endl;
    MainWindow::defaultDownloadDirectory.cdUp();
    if(!success){
        //wait for timeout
        QTimer::singleShot(25000,this,[=]{
            callSinglePage(chapter,page,seriesName,maxPages,base_url);
        });
        return;
    }



    if(page < maxPages){
        double x1 = page;
        double x2 = chapter;
        QTimer::singleShot(150,this,[=]{
            ui->progressBar->setValue(
                    (((x2/10) -ui->minspin->value() + (x1/maxPages))*100/((ui->maxspin->value() - ui->minspin->value())))
                    );
            callSinglePage(chapter,page + 1,seriesName,maxPages,base_url);
        });
    }else
    if(page == maxPages){
        //GENERATE PDF
        //wait to be sure
        QTimer::singleShot(2000,this,[=]{
            HPDF_Doc pdf;
            HPDF_Error_Handler error_handler = HPDF_Error_Handler();
            pdf = HPDF_New(error_handler,NULL);
            if (!pdf) {
                printf ("ERROR: cannot create pdf object.\n");
                return;
            }
            HPDF_SetPageMode (pdf, HPDF_PAGE_MODE_USE_OUTLINE);

            //scroll thru pages:)
            MainWindow::defaultDownloadDirectory.cd(QString::fromStdString(seriesName) + "-"+QString::fromStdString(numToChapter2(chapter)));
            for(int i = 1;i<= maxPages;i++){
                auto img = HPDF_LoadPngImageFromFile(pdf,(MainWindow::defaultDownloadDirectory.path()+"/" +QString::fromStdString(seriesName)
                                               + "-"+QString::fromStdString(numToChapter2(chapter))+"-"
                                               +QString::fromStdString(numToChapter(i))+".png").toStdString().c_str());


                HPDF_Page page_1;
                page_1 = HPDF_AddPage (pdf);
                HPDF_Page_SetWidth(page_1,HPDF_Image_GetWidth(img));
                HPDF_Page_SetHeight(page_1,HPDF_Image_GetHeight(img));
                HPDF_Page_DrawImage (page_1,img,0, 0,HPDF_Image_GetWidth(img),HPDF_Image_GetHeight(img));
            }




            MainWindow::defaultDownloadDirectory.cdUp();
            std::string savestring =  MainWindow::defaultDownloadDirectory.path().toStdString() +
                    '/' + seriesName
                    + "-"+ numToChapter2(chapter)+".pdf";

            std::cout<<savestring<< " " <<HPDF_SaveToFile(pdf,savestring.c_str())<<std::endl;
            HPDF_Free(pdf);
        });



        //NEXT CHAPTER
         QTimer::singleShot(5000,this,[=]{
             std::cout<<"newChapter"<<std::endl;
             enterMainLoop();});

//             ui->progressBar->setValue(
//                     ((chapter/10) - ui->minspin->value())*100/(ui->maxspin->value() - ui->minspin->value())
//                     );
    }

}

void DownloadPopup::callSinglePage(int chapter, int page, std::string seriesName, int maxPages,std::string base_url)
{
    std::string chapstr;
    std::cout<<"qwe?"<<std::endl;
    chapstr = numToChapter2(chapter/10);
    if(chapter % 10 != 0){
        chapstr += "." + std::to_string(chapter%10);
    }
    std::cout<<chapstr<<std::endl;
    ui->textBrowser->append("Downloading page: " + QString::fromStdString(std::to_string(page)));
    QString url = "https://"+QString::fromStdString(base_url) + "/manga/" + QString::fromStdString(info->getName())
            + "/" + QString::fromStdString(chapstr)+'-'+QString::fromStdString(numToChapter(page))+".png";
    std::cout<<url.toStdString()<<std::endl;
    //Download page after waiting for some time
    auto man = new QNetworkAccessManager();
    connect(man,&QNetworkAccessManager::finished,this,[=](QNetworkReply * res){
        savePage(res,chapter,page,seriesName,maxPages,base_url);
    });
    man->get(QNetworkRequest(QUrl(url)));

}

void DownloadPopup::on_minspin_valueChanged(int arg1)
{
    ui->maxspin->setMinimum(arg1 + 1);
}


void DownloadPopup::on_maxspin_valueChanged(int arg1)
{
    ui->minspin->setMaximum(arg1 - 1);
}


void DownloadPopup::on_pushButton_clicked()
{
    //first go to the main page, parse that html
    QString url = MainWindow::domainLink + "/manga/" + QString::fromStdString(info->getName());
//    std::cout<<url.toStdString()<<std::endl;
    auto man = new QNetworkAccessManager();
    connect(man,SIGNAL(finished(QNetworkReply *)),this,SLOT(gotMainPage(QNetworkReply *)));
    man->get(QNetworkRequest(QUrl(url)));
}

void DownloadPopup::gotMainPage(QNetworkReply *res)
{
    std::string source = res->readAll().toStdString();
    size_t t1 = source.find("vm.Chapters = ") + 14;
    size_t t2 = source.find(';',t1);
    source = source.substr(t1,t2-t1);
//    std::cout<<source<<std::endl;
    QJsonDocument doc = QJsonDocument::fromJson(QString(source.c_str()).toUtf8());
    arr = doc.array();
    maxChapters = arr.size();
//    std::cout<<arr.size()<<"  arsize"<<std::endl;
    enterMainLoop();
}

void DownloadPopup::gotChapterPage(QNetworkReply *res,int chapter)
{
    std::string source = res->readAll().toStdString();
    int t1 = source.find("vm.CurPathName = ") + 17;
    int t2 = source.find(';',t1);
    std::string base_url = source.substr(t1+1,t2-t1-2);
    t1 = source.find("vm.CurChapter = ") + 16;
    t2 = source.find(';',t1);
    std::string chapterInfoJsonText = source.substr(t1,t2-t1);
    t1 = source.find("vm.IndexName = \"") + 16;
    t2 = source.find('"',t1);
    std::string seriesName = source.substr(t1,t2-t1);


    QJsonDocument doc = QJsonDocument::fromJson(QString(chapterInfoJsonText.c_str()).toUtf8());
    QJsonObject obj = doc.object();
    int pages = obj.value("Page").toString().toInt();
//    std::string ustr = res->url().toString().toStdString();
//    t1 = ustr.rfind("-chapter-") + 9;
//    t2 = ustr.rfind(".html");
//    ustr = ustr.substr(t1,t2-t1);
//    t1 = ustr.find('.');
//    int chapter = 0;
//    if(t1 == -1) chapter = QString::fromStdString(ustr).toInt() * 10;
//    else{
//        chapter = (10*(QString::fromStdString(ustr.substr(0,t1)).toInt())) + ustr[ustr.length()-1];
//    }

    callSinglePage(chapter,1,seriesName,pages,base_url);


}


