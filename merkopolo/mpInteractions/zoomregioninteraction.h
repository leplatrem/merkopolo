#ifndef ZOOMREGIONINTERACTION_H
#define ZOOMREGIONINTERACTION_H

#include <QPoint>

#include "baseinteraction.h"


class MPMapView;

class ZoomRegionInteraction : public BaseInteraction
{
    Q_OBJECT
public:
    explicit ZoomRegionInteraction(MPMapView* aView);
    virtual QCursor cursor() const;

    virtual bool isSnapEnabled();
    void paintEvent(QPaintEvent *event, QPainter &thePainter);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

protected:
    /*! Dragging start corner */
    QPoint P1;
    /*! Dragging end corner */
    QPoint P2;
    /*! Distinguish a mouse move from a mouse drag */
    bool m_dragging;
};

#endif // ZOOMREGIONINTERACTION_H
