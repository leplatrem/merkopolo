#ifndef MPMAPVIEW_H
#define MPMAPVIEW_H

#include <QMutex>

#include "MapView.h"

#define VIEWPORT_SHIFT_PERCENT 0.75

class MPWindow;
class LayerSwitcher;
class Interaction;


class MPMapView : public MapView
{
    Q_OBJECT

public:
    explicit MPMapView(MPWindow *parent = 0);

    void updateDefaultCursor();
    void setDocument(Document*);
    virtual Interaction * defaultInteraction();
    virtual void launch(Interaction *anInteraction);

    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent*);

signals:
    void viewportShift();
    void mouseMove(QMouseEvent*);
    void featureSnap(Feature*);
    void painted(qlonglong);
    void imageRequested(int);
    void imageReceived();
    void imageFinished();

protected slots:
    void invalidateAll();
    void on_userIdle();
    void on_featureSnap(Feature*);
    void on_imageRequested(ImageMapLayer*);
    void on_imageReceived(ImageMapLayer*);
    void on_loadingFinished(ImageMapLayer*);

protected:
    /*! Pointer to main window application */
    MPWindow* m_window;
    /*! Pointer to layer switcher */
    LayerSwitcher* m_layerswitcher;

    /*! Number of images currently downloading */
    int m_numImages;
    /*! Previous viewport, last time the user was idle */
    CoordBox m_previousviewport;
};

#endif // MPMAPVIEW_H
