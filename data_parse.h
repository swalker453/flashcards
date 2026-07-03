#ifndef DATA_PARSE_H
#define DATA_PARSE_H

#include <QTreeWidgetItem>
#include <QMainWindow>

class csv_record{
public:
    int line_num;
    int no;
    int kind;
    std::vector<QString> attr;
    int correct_num;
    int wrong_num;

    csv_record(int num,QString data);
    QString emit_data();
};

class csv_parse{
public:
    void init(QWidget *parent, QString Filename);
    void write(QWidget *parent);
    void cor_wor_increment(int line_num, bool correct);
    std::vector<csv_record *> csv_records;
    csv_record* record_get(int line_num);
    std::vector<int> exist_line_get();
private:
    QString filename;
};


class disp_question : public QObject{
    Q_OBJECT
    Q_PROPERTY(int line_num MEMBER m_line_num NOTIFY dataChanged)
    Q_PROPERTY(int no MEMBER m_no NOTIFY dataChanged)
    Q_PROPERTY(int correct_num MEMBER m_correct_num NOTIFY dataChanged)
    Q_PROPERTY(int wrong_num MEMBER m_wrong_num NOTIFY dataChanged)
    Q_PROPERTY(QString kind MEMBER m_kind NOTIFY dataChanged)
    Q_PROPERTY(QString question MEMBER m_question NOTIFY dataChanged)
    Q_PROPERTY(QString q_sentence MEMBER m_q_sentence NOTIFY dataChanged)
    Q_PROPERTY(QString answer MEMBER m_answer NOTIFY dataChanged)
    Q_PROPERTY(QString a_sentence MEMBER m_a_sentence NOTIFY dataChanged)

public:
    explicit disp_question(QObject *parent = nullptr) : QObject(parent) {
        m_line_num = 0;
        m_no=0;
        m_kind = "読み";
        m_question = "テスト";
        m_q_sentence = "テストです";
        m_answer="テスト答え";
        m_a_sentence="テスト答えです";
        m_correct_num=0;
        m_wrong_num = 0;
    }

    int m_line_num;
    int m_no;
    QString m_kind;
    QString m_question;
    QString m_q_sentence;
    QString m_answer;
    QString m_a_sentence;
    int m_correct_num;
    int m_wrong_num;

signals:
    void dataChanged();

};

#endif // DATA_PARSE_H
