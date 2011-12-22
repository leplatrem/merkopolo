#ifndef MPWINDOW_H
#define MPWINDOW_H

#include <QMainWindow>
#include <QtToolBarDialog>
#include <QProgressBar>
#include <QLabel>

#include "Coord.h"

namespace Ui {
    class MPWindow;
}

class Feature;
class Interaction;
class ImageMapLayer;

class MPDocument;
class MPMapView;
class InfosDock;
class BaseLayer;
class CoordField;

class MPWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MPWindow(QWidget *parent = 0);
    ~MPWindow();
    void loadDocument(MPDocument *aDoc);

signals:
    void interactionReinitialize();
    void close();

public slots:
    void invalidateView();
    void onCenterView(qreal,qreal);

    // Actions slots from UI
    void displayInfosDock();
    void onDisplayInfosDock(bool);
    void viewZoomIn();
    void viewZoomOut();
    void viewZoomWindow();

    // Application slots
    void onViewShift();
    void onViewMouseMove(QMouseEvent *event);
    void onViewFeatureSnap(Feature *feature);
    void onViewPainted(qlonglong);
    void onViewImageRequested(int nbrequested);
    void onViewImageReceived();
    void onViewImageFinished();
    void onInteractionChanged(Interaction *interaction);

protected slots:
    void showCriticalError(QString);
    void showWarningError(QString);
    void loadSettings();
    void saveSettings();

protected:
    void closeEvent(QCloseEvent* event);
    void initUIComponents();
    void setMapViewport(const CoordBox&, bool wider = true, bool force = false, bool invalidate = true);

    /*! Pointer to main map view */
    MPMapView* m_view;
    /*! Pointer to main document displayed */
    MPDocument* m_document;
    /*! Pointer to street background layer */
    ImageMapLayer* m_streetlayer;

    /*! A dock to display hovered features informations */
    InfosDock* m_infosdock;
    /*! Temporary vieport coordinates status text */
    CoordField* m_coordsLabel;
    /*! Status text for map view paint duration */
    QLabel* m_paintTimeLabel;
    /*! Status text for map view zoom level in pixels/meter */
    QLabel* m_meterPerPixelLabel;
    /*! Status text for map view zoom level as integer  */
    QLabel* m_zoomlevelLabel;
    /*! Status progressbar for images (tiles) loading  */
    QProgressBar* m_imagesProgress;
    /*! Status progressbar for data loading */
    QProgressBar* m_dataProgress;
    /*! Status progressbar for webservice requests */
    QProgressBar* m_wsProgress;
    /*! Separator between status and zoom */
    QFrame* m_sepCoordZoom;
    /*! Separator between zoom an scale */
    QFrame* m_sepZoomScale;
    /*! Separator between scale and time */
    QFrame* m_sepScaleTime;

private:
    /*! Pointer to UI window form */
    Ui::MPWindow* ui;
};

#endif // MPWINDOW_H

