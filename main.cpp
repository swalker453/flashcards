#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QIcon>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
#endif

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/image/ico3.png"));

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "flashcards_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
#ifdef Q_OS_ANDROID
    w.showMaximized();
#else
    w.show();
#endif
    return QApplication::exec();
}
