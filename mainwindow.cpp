#include "mainwindow.h"
#include "data_parse.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QQmlContext>
#include <QQuickItem>
#include <algorithm>
#include <random>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    not_go_next= 0;
    questlineUpdatePending = false;

    ui->setupUi(this);

#ifdef Q_OS_ANDROID
    ui->toolBar->hide();
#endif
    \
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

    ui->treeWidget->blockSignals(true);
    QStringList header_str;
    header_str << "No." << "問題" << "問題例" << "正解数" << "誤答数";
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



    ui->treeWidget->collapseAll();
    ui->treeWidget->blockSignals(false);

    QSurfaceFormat fmt;
    fmt.setAlphaBufferSize(8);
    ui->quickWidget->setFormat(fmt);
    ui->quickWidget->setClearColor(Qt::transparent);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->quickWidget->setAttribute(Qt::WA_TranslucentBackground);

    ui->quickWidget->setAttribute(Qt::WA_AlwaysStackOnTop, true);

    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    ui->quickWidget->rootContext()->setContextProperty("mainWindow", this);
    ui->quickWidget->rootContext()->setContextProperty("dq",&dq);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/tester.qml")));

    ui->label_4->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_4->setOpenExternalLinks(true);

    questline_set();
    disp_refresh();

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
        "<h2>熊の字印の漢検準一級暗記帳</h2>"
        "<p>Version 1.0.0</p>"
        "<p>漢検準一級に限る必要は無い気がします</p>"
        "<p><i>© 2026 oikawhr</i></p>"
        );
}


void MainWindow::on_pushButton_2_clicked()
{
    QString fileName;

#ifdef Q_OS_ANDROID
    fileName = QFileDialog::getOpenFileName(
        this,
        "出題用CSVファイルを開く"
        );
#else
    fileName = QFileDialog::getOpenFileName(
        this,
        "出題用CSVファイルを開く",
        "",
        "問題CSVファイル (*.csv)"
        );
#endif

    if (!fileName.isEmpty()) {
#ifndef Q_OS_ANDROID
        if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "ファイル形式エラー", "CSVファイルを選択してください。");
            return;
        }
#endif
        ui->lineEdit->setText(fileName);
    }
    on_pushButton_clicked();
}


void MainWindow::on_pushButton_clicked()
{
    body.init(this, ui->lineEdit->text());
    disp_refresh();
    questline_set(1);
    question_set();
}

void MainWindow::questline_set(int reset){
    std::vector<int> a,b;
    QObject *root = ui->quickWidget->rootObject();

    if(!root) return;
    int now_line_before;

    if(reset){
        now_line_before = 0;
    }
    else{
        now_line_before = dq.m_no;
    }
    not_go_next = 0;

    a=body.exist_line_get();

    QTreeWidgetItemIterator it(ui->treeWidget);

    while (*it) {
        QTreeWidgetItem *item = *it;
        if(item->checkState(0) == Qt::Checked && item->data(0,kindRole) == 2){
            if(ui->checkBox_2->isChecked() || item->text(3).toInt()<=0){
                b.push_back(item->data(0,lineRole).toInt());
            }
        }

        ++it;
    }

    quest_lines.clear();
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    std::set_intersection(
        a.begin(), a.end(),
        b.begin(), b.end(),
        std::back_inserter(quest_lines)
        );

    if(ui->checkBox->isChecked()){
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(quest_lines.begin(),quest_lines.end(),g);
        now_line = quest_lines.begin();
    }
    else{

        std::sort(quest_lines.begin(), quest_lines.end(),
                  [&](const int a, const int b){
                      return body.record_get(a)->no < body.record_get(b)->no;
                  });
        now_line = quest_lines.begin();

        for (std::vector<int>::iterator it = quest_lines.begin(); it != quest_lines.end(); ++it) {
            if(body.record_get(*it)->no<now_line_before){
                now_line = it;
            }
            else if(body.record_get(*it)->no==now_line_before){
                now_line = it;
                break;
            }
            else {
                if(!reset){
                    not_go_next = 1;
                }
                break;
            }
        }
    }


    if(quest_lines.size()>0){
        if(root->property("status")==0){
            question_set();
        }
        root->setProperty("status",1);
    }
    else{
        root->setProperty("status",0);
        root->setProperty("answer_status",0);
        question_set();
    }


}

void MainWindow::disp_refresh(){
    int quest_many[GENRES_MANY];
    for(int i=0;i<GENRES_MANY;++i){
        quest_many[i] = 0;
    }

    ui->treeWidget->blockSignals(true);
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

            quest_many[(*it)->kind-1]++;
        }
    }

    QStringList genres_str;
    genres_str << "読み" << "表外読み" << "熟語一字訓" << "書き取り" <<"四字熟語" <<"対義語類義語" << "故事諺";
    for(int i=0; i<GENRES_MANY; i++){
        genres.at(i)->setText(0, genres_str.at(i) + " （" + QString::number(quest_many[i]) + "問）");
    }



    ui->treeWidget->collapseAll();
    ui->treeWidget->blockSignals(false);

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

void MainWindow::question_set(){
    csv_record *tmp_record;

    if(quest_lines.empty() || body.csv_records.empty()){
        dq.m_question = "";
        dq.m_q_sentence = "";
        dq.m_answer = "";
        dq.m_a_sentence = "";
        dq.m_line_num = 0;
        dq.m_no = 0;
        dq.m_correct_num = 0;
        dq.m_wrong_num = 0;
        dq.m_kind = "";

        emit dq.dataChanged();
        return;
    }

    tmp_record = body.record_get(*now_line);


    QStringList genres_str;
    genres_str << "読み" << "表外読み" << "熟語一字訓" << "書き取り" <<"四字熟語" <<"対義語類義語" << "故事諺";
    \
    dq.m_question = tmp_record->attr.at(0);
    QString temp_qs = tmp_record->attr.at(1);
    dq.m_q_sentence = temp_qs.replace("<","~").replace(">","</u>").replace("~","<u>");
    dq.m_answer = tmp_record->attr.at(2);
    dq.m_a_sentence = tmp_record->attr.at(3);
    dq.m_line_num = tmp_record->line_num;
    dq.m_no = tmp_record->no;
    dq.m_correct_num = tmp_record->correct_num;
    dq.m_wrong_num = tmp_record->wrong_num;

    if(tmp_record->kind>0 &&  tmp_record->kind<=7){
        dq.m_kind = genres_str.at(tmp_record->kind-1);
    }
    else{
        dq.m_kind = "";
    }

    QObject *root = ui->quickWidget->rootObject();
    root->setProperty("answer_status",0);

    emit dq.dataChanged();
}

void MainWindow::questionChangeClicked(bool correct){

    if(quest_lines.empty() || body.csv_records.empty()){
        return;
    }

    if(correct){
        body.cor_wor_increment(dq.m_line_num,1);

    }else{
        body.cor_wor_increment(dq.m_line_num,0);

    }

    QTreeWidgetItemIterator it(ui->treeWidget);

    ui->treeWidget->blockSignals(true);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if(item->data(0,lineRole).toInt() == dq.m_line_num && item->data(0,kindRole).toInt() == 2){
            if(correct){
                (*it)->setText(3, QString::number(body.record_get(dq.m_line_num)->correct_num));
            }else{
                (*it)->setText(4, QString::number(body.record_get(dq.m_line_num)->wrong_num));
            }
        }

        ++it;
    }
    ui->treeWidget->blockSignals(false);

    if(!not_go_next){
        now_line++;
    }
    not_go_next = 0;
    if(now_line == quest_lines.end()){
        questline_set(1);
    }


    body.write(this);
    question_set();

}

void MainWindow::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) return;
    if (item->data(0, kindRole).toInt() != 2) return;

    if (questlineUpdatePending) return;
    questlineUpdatePending = true;

    QTimer::singleShot(0, this, [this]() {
        questlineUpdatePending = false;
        questline_set();
    });
}


void MainWindow::on_checkBox_checkStateChanged(const Qt::CheckState &arg1)
{
    questline_set();
}


void MainWindow::on_checkBox_2_checkStateChanged(const Qt::CheckState &arg1)
{
    questline_set();
}


void MainWindow::on_pushButton_3_clicked()
{
    body.cor_wor_reset();

    ui->treeWidget->blockSignals(true);

    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        QTreeWidgetItem *item = *it;
        if(item->data(0,kindRole).toInt() == 2){
            (*it)->setText(3, QString::number(0));
            (*it)->setText(4, QString::number(0));
        }

        ++it;
    }
    ui->treeWidget->blockSignals(false);

    body.write(this);
}


void MainWindow::on_checkBox_3_checkStateChanged(const Qt::CheckState &arg1)
{
    QObject *root = ui->quickWidget->rootObject();
    root->setProperty("soundon",arg1 == Qt::Checked);
}

