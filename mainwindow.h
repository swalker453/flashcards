#define GENRES_MANY 7

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTreeWidgetItem>
#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include "data_parse.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setCursor(Qt::PointingHandCursor);
        setMouseTracking(true);
    }

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }

        QLabel::mouseReleaseEvent(event);
    }

    void enterEvent(QEnterEvent *event) override
    {
        hover = true;
        update();
        QLabel::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override
    {
        hover = false;
        update();
        QLabel::leaveEvent(event);
    }

    void paintEvent(QPaintEvent *event) override
    {
        QLabel::paintEvent(event);

        if (hover) {
            QPainter painter(this);
            painter.fillRect(rect(), QColor(255, 255, 255, 127));
        }
    }

private:
    bool hover = false;
};

#endif // CLICKABLELABEL_H

enum {
    kindRole = Qt::UserRole + 1,
    lineRole,
    answerRole,
    kaisetsuRole
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void questline_set(int reset=0);
    void question_set();
    Q_INVOKABLE void questionChangeClicked(bool correct);

private slots:
    void on_action_X_triggered();

    void on_action_V_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_checkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_checkBox_2_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButton_3_clicked();

    void on_checkBox_3_checkStateChanged(const Qt::CheckState &arg1);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    ClickableLabel *bg_image2 = nullptr;
    disp_question dq;

    std::vector<int>::iterator now_line;
    std::vector<int> quest_lines;
    csv_parse body;

    std::vector<QTreeWidgetItem *>genres;
    std::vector<QTreeWidgetItem *>questions;

    void disp_refresh();
    int not_go_next;
    bool questlineUpdatePending;

};
#endif // MAINWINDOW_H
