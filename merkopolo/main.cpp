#include <QtGui/QApplication>
#include <QMessageBox>
#include <QTranslator>

#include "mpwindow.h"

#define  LOCALE_DIR  "locale"
#define  LOCALE_FILE "merkopolo-%1"


/*! \mainpage Source Code Documentation
 */


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Merkopolo");
    QCoreApplication::setApplicationName("Merkopolo");

    /*
     * Start application !
     */

    // Load language file
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load(QString(LOCALE_FILE).arg(locale), LOCALE_DIR)) {
        a.installTranslator(&translator);
        qDebug() << QApplication::tr("Using lang '%1'").arg(locale);
    }
    else {
        qDebug() << QApplication::tr("Could not load lang '%1'").arg(locale);
    }

    // Run application
    MPWindow* w = new MPWindow;
    w->show();
    return a.exec();
}
