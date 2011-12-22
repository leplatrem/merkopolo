#ifndef BASEDOCK_H
#define BASEDOCK_H

#include "MDockAncestor.h"

class BaseDock : public MDockAncestor
{
    Q_OBJECT
public:
    explicit BaseDock(QWidget *parent = 0);
    ~BaseDock();

    /*! Reload UI after application language is changed. \note not used. */
    virtual void retranslateUi() {};
    void closeEvent(QCloseEvent * event);

signals:
    void dockClosed(bool);
};

#endif // BASEDOCK_H
