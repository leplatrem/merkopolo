#ifndef COORDFIELD_H
#define COORDFIELD_H

#include <QWidget>
#include <QLineEdit>

class CoordField : public QLineEdit {
    Q_OBJECT

public:
    CoordField(QWidget *parent = 0);
    ~CoordField();

    void setCoord(QPointF);

signals:
    void centerView(qreal, qreal);

public slots:
    void coordsSet();
    void textEdited(QString);

protected:
    void setError(bool);
    bool verifCoords(QString, qreal&, qreal&);

};

#endif // COORDFIELD_H
