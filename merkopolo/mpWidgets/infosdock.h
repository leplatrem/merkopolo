#ifndef INFOSDOCK_H
#define INFOSDOCK_H

#include <QString>
#include <QTextBrowser>

#include "basedock.h"

class InfosDock : public BaseDock
{
    Q_OBJECT
public:
    explicit InfosDock(QWidget *parent = 0);
    ~InfosDock();

    void setHtml(QString html);
    QString getHtml();
    void setHoverHtml(QString html);
    void unsetHoverHtml();

private:
    /*! A widget to display HTML content. */
    QTextBrowser* m_text;
    /*! Current HTML content. */
    QString m_currentHtml;
};

#endif // INFOSDOCK_H
