#include "data_parse.h"
#include "mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

csv_record::csv_record(int num, QString data) : line_num(0),no(0), kind(0), correct_num(0), wrong_num(0){
    QStringList cols = data.split(',');
    if(cols.size()!=8){
        return;
    }
    else{
        bool change_check[4] = {false,false,false,false};
        no = cols.at(0).toInt(&change_check[0]);
        kind = cols.at(1).toInt(&change_check[1]);
        correct_num = cols.at(6).toInt(&change_check[2]);
        wrong_num = cols.at(7).toInt(&change_check[3]);
        for(int i=0;i<4;i++){
            if(change_check[i] == false){
                line_num=0;
                return;
            }
        }

        attr.push_back(cols.at(2));
        attr.push_back(cols.at(3));
        attr.push_back(cols.at(4));
        attr.push_back(cols.at(5));

        line_num = num;
    }
}

QString csv_record::emit_data(){

    return "";
}

void csv_parse::init(QWidget* parent, QString Filename){
    filename= Filename;
    QFile file(filename);

    for (std::vector<csv_record*>::iterator it = csv_records.begin(); it != csv_records.end(); it++) {
        csv_record* p = *it;
        delete p;
    }
    csv_records.clear();
    csv_records.shrink_to_fit();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(
            parent,"警告","ファイルを開けませんでした。");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    int line_num=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        line_num++;
        csv_record *push_tmp = new csv_record(line_num, line);
        if(push_tmp->line_num==line_num){
            csv_records.push_back(push_tmp);
        }
        else{
            delete push_tmp;
        }
    }

    std::sort(csv_records.begin(), csv_records.end(),
        [](const csv_record *a, const csv_record *b){
        return a->no < b->no;
    });

}
void csv_parse::write(){

}

csv_record* csv_parse::record_get(int line_num){
    for (std::vector<csv_record *>::iterator it = csv_records.begin();
         it != csv_records.end();
         ++it)
    {
        if(line_num == (*it)->line_num) return (*it);
    }


    return nullptr;
}

std::vector<int> csv_parse::exist_line_get(){
    std::vector<int> ret_lines_tmp;
    ret_lines_tmp.clear();

    for (std::vector<csv_record *>::iterator it = csv_records.begin();
         it != csv_records.end();
         ++it)
    {

        ret_lines_tmp.push_back((*it)->line_num);
    }

    std::sort(ret_lines_tmp.begin(), ret_lines_tmp.end());
    return ret_lines_tmp;

}