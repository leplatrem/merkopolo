#ifndef BASEINTERACTION_H_
#define BASEINTERACTION_H_

#include <QTimer>

#include "Interaction.h"

#define IDLE_TIMEOUT 750

class Layer;
class Feature;

class MPMapView;

class BaseInteraction :	public FeatureSnapInteraction
{
    Q_OBJECT

public:
    explicit BaseInteraction(MPMapView* theView);
    ~BaseInteraction(void);

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void wheelEvent(QWheelEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent*);

    virtual void paintEvent(QPaintEvent* anEvent, QPainter& thePainter);
    virtual void updateSnap(QMouseEvent *event);
    virtual QString toHtml();

    void setSnapEnabled(bool);
    virtual bool isSnapEnabled();
    virtual void handleLoadLayerDone(Layer*){}

public slots:
    virtual void reinitialize();

signals:
    void idle();

public slots:
    void onTimerTimeout();

protected:
    void resetIdleTimer();

    /*! Store if snap is enabled */
    bool m_snapEnabled;
    /*! Simple timout timer to be easily reset */
    QTimer* m_idletimer;

};

#endif  // BASEINTERACTION_H_
