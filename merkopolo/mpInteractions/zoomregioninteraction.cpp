#include "zoomregioninteraction.h"

#include <QPixmap>
#include <QCursor>
#include <QPainter>

#include "mpmapview.h"


/*! \class ZoomRegionInteraction
    \brief A custom zoom region interaction to provide a nicer cursor
 */

/*! Constructs a ZoomRegionInteraction
  */
ZoomRegionInteraction::ZoomRegionInteraction(MPMapView* aView) :
    BaseInteraction(aView),
    m_dragging(false)
{
    aView->setCursor(cursor());
}

/*! Provide a mouse cursor for this interaction
  */
QCursor ZoomRegionInteraction::cursor() const
{
    QPixmap pm(":/mpIcons/MCursorZoomRegion.svg");
    return QCursor(pm,17,17);
}

/*! Override to always disable snapping with this interaction
  */
bool ZoomRegionInteraction::isSnapEnabled()
{
    return false;
}

void ZoomRegionInteraction::paintEvent(QPaintEvent* event, QPainter& thePainter)
{
    Q_UNUSED(event);

    if (m_dragging)
    {
        QPen TP(Qt::DashDotLine);
        thePainter.setBrush(Qt::NoBrush);
        TP.setColor(Qt::red);
        thePainter.setPen(TP);
        thePainter.drawRect(QRectF(P1,QSize(int(P2.x()-P1.x()),int(P2.y()-P1.y()))));
    }
}

void ZoomRegionInteraction::mousePressEvent(QMouseEvent * event)
{
    P1 = P2 = event->pos();
    m_dragging = true;
}

void ZoomRegionInteraction::mouseReleaseEvent(QMouseEvent * event)
{
    P2 = event->pos();
    m_dragging = false;

    CoordBox coordbox(XY_TO_COORD(P1), XY_TO_COORD(P2));
    if (!coordbox.isEmpty()) {
        view()->setViewport(coordbox, view()->rect());
        view()->invalidate(true, true);
        view()->launch(NULL);
    }
}

void ZoomRegionInteraction::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging)
    {
        P2 = event->pos();
        view()->update();
    }
}
