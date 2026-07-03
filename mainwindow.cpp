#include "mainwindow.h"
#include "data_parse.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bg_image2 = new ClickableLabel(ui->centralwidget);
    bg_image2->setPixmap(QPixmap(":/image/image/banner.png"));
    bg_image2->setScaledContents(true);
    bg_image2->setFixedSize(200,40);
    bg_image2->setStyleSheet("background:transparent;");
    connect(bg_image2, &ClickableLabel::clicked, this, []() {
        QDesktopServices::openUrl(QUrl("https://swalker.sakura.ne.jp/"));
    });

    QString sourceDir = QCoreApplication::applicationDirPath();
    ui->lineEdit->setText(sourceDir + "/source.csv");
    ui->lineEdit->setReadOnly(true);

    ui->toolBar->setMovable(false);
    ui->toolBar->setFloatable(false);

    ui->treeWidget->setHeaderHidden(false);

    QStringList header_str;
    header_str << "No." << "問題" << "問題例" << "正解数" << "間違数";
    ui->treeWidget->setHeaderLabels(header_str);
    ui->treeWidget->setColumnCount(5);

    QHeaderView* header = ui->treeWidget->header();
    header->setStretchLastSection(false);
    ui->treeWidget->setColumnWidth(0, 100);
    ui->treeWidget->setColumnWidth(1, 200);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->treeWidget->setColumnWidth(3, 50);
    ui->treeWidget->setColumnWidth(4, 50);

    QStringList genres_str;
    genres_str << "読み" << "表外読み" << "熟語一字訓" << "書き取り" <<"四字熟語" <<"対義語類義語" << "故事諺";
    for(int i=0; i<GENRES_MANY; i++){
        genres.push_back(new QTreeWidgetItem(ui->treeWidget));
        genres.back()->setText(0, genres_str.at(i));
        genres.back()->setFlags(genres.back()->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
        genres.back()->setFirstColumnSpanned(true);
        genres.back()->setCheckState(0, Qt::Checked);

        genres.back()->setData(0, kindRole, 1);

        QFont font = genres.back()->font(0);
        font.setBold(true);
        font.setUnderline(true);
        genres.back()->setFont(0,font);
    }



    ui->treeWidget->expandAll();

    QSurfaceFormat fmt;
    fmt.setAlphaBufferSize(8);
    ui->quickWidget->setFormat(fmt);
    ui->quickWidget->setClearColor(Qt::transparent);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->quickWidget->setAttribute(Qt::WA_TranslucentBackground);

    ui->quickWidget->setAttribute(Qt::WA_AlwaysStackOnTop, true);

    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    ui->quickWidget->rootContext()->setContextProperty("dq",&dq);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/tester.qml")));



}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    bg_image2->move(ui->centralwidget->width()-210,10);
    bg_image2->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_X_triggered()
{
    QApplication::quit();
}


void MainWindow::on_action_V_triggered()
{
    QMessageBox::about(
        this,
        "バージョン情報",
        "<h2>熊の字の漢検準一級暗記帳</h2>"
        "<p>Version 1.0.0</p>"
        "<p>漢検準一級に限る必要は無い気がします</p>"
        "<p><i>© 2026 oikawhr</i></p>"
        );
}


void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "問題ファイルを開く",
        "",
        "問題CSVファイル (*.csv)"
        );
    if (!fileName.isEmpty()) {
        ui->lineEdit->setText(fileName);
    }
    on_pushButton_clicked();
}


void MainWindow::on_pushButton_clicked()
{
    body.init(this, ui->lineEdit->text());
    disp_refresh();
}

void MainWindow::disp_refresh(){



    for(int i=0;i<GENRES_MANY;i++){
        qDeleteAll(genres.at(i)->takeChildren());
    }
    questions.clear();

    for (std::vector<csv_record*>::iterator it = body.csv_records.begin(); it != body.csv_records.end(); it++) {
        if((*it)->kind>=1 && (*it)->kind<=GENRES_MANY){
            questions.push_back(new QTreeWidgetItem(genres.at((*it)->kind-1)));
            questions.back()->setText(0, QString::number((*it)->no));
            questions.back()->setText(1, (*it)->attr.at(0));
            questions.back()->setText(2, (*it)->attr.at(1));
            questions.back()->setText(3, QString::number((*it)->correct_num));
            questions.back()->setText(4, QString::number((*it)->wrong_num));

            questions.back()->setData(0, kindRole, 2);
            questions.back()->setData(0, lineRole, (*it)->line_num);
            questions.back()->setData(0, answerRole, (*it)->attr.at(2));
            questions.back()->setData(0, kaisetsuRole, (*it)->attr.at(3));

            questions.back()->setFlags(questions.back()->flags() | Qt::ItemIsUserCheckable);
            questions.back()->setCheckState(0, Qt::Checked);

        }
    }

    ui->treeWidget->expandAll();

}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if( item->data(0,kindRole).toInt()==2){
        QMessageBox::information(
            this,
            item->text(1),
            "<h1>"+ item->data(0,answerRole).toString() + "</h1>\n<p>" + item->data(0,kaisetsuRole).toString() + "</p>" + "<p>CSV行番号 : " + item->data(0,lineRole).toString() + "</p>"
            );
    }
}

