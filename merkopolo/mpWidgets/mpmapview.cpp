#include "mpmapview.h"

#include <QPainter>
#include "Document.h"
#include "LayerIterator.h"
#include "ImageMapLayer.h"

#include "mpwindow.h"
#include "baseinteraction.h"
#include "layerswitcher.h"

/*!
  \class MPMapView
  \brief The main map widget.

  Extension of Merkaartor's MapView with custom signals and
  default style and rendering options.
*/

/*! \fn void MPMapView::viewportShift()
  This signal is emitted when the viewport has suffered a big move (since last idle())
  */
/*! \fn void MPMapView::mouseMove(QMouseEvent*)
  This signal is emitted when the mouse has moved.
  */
/*! \fn void MPMapView::featureSnap(Feature*)
  This signal is emitted when a feature is snapped (hovered).
  */
/*! \fn void MPMapView::painted(qlonglong)
  This signal is emitted when the map view is painted.
  */
/*! \fn void MPMapView::imageRequested(int)
  This signal is emitted when a image (tile) download is requested.
  */
/*! \fn void MPMapView::imageReceived()
  This signal is emitted when a image (tile) download is received.
  */
/*! \fn void MPMapView::imageFinished()
  This signal is emitted when all images (tiles) downloads are finished.
  */


/*! Constructs the map widget.
  */
MPMapView::MPMapView(MPWindow *parent) :
    MapView(parent),
    m_window(parent),
    m_layerswitcher(0),
    m_numImages(0)
{
    m_layerswitcher = new LayerSwitcher(this);
    connect(m_layerswitcher, SIGNAL(layerSwitched()), this, SLOT(invalidateAll()));

    // Hide intermediary points on ways
    M_PREFS->setTrackPointsVisible(false);

    // Default rendering options
    RendererOptions opt;
    opt.options |= RendererOptions::ScaleVisible;
    opt.options |= RendererOptions::LatLonGridVisible;
    opt.options |= RendererOptions::BackgroundVisible;
    opt.options |= RendererOptions::ForegroundVisible;
    opt.options |= RendererOptions::TouchupVisible;    // layer for annotations
    opt.options |= RendererOptions::UnstyledHidden;    // do not draw all nodes
    opt.options |= RendererOptions::NamesVisible;
    //opt.options |= RendererOptions::LockZoom;          // lock zoom to tile levels
    setRenderOptions(opt);

    // false : middle button panning
    // true : left button panning
    M_PREFS->setMouseSingleButton(true);

    // Do not draw features parents
    M_PREFS->setShowParents(false);

    M_PREFS->setZoomIn(200);
    M_PREFS->setZoomOut(50);
}

/*! Rely on the current interaction's cursor to update the current view cursor.
  */
void MPMapView::updateDefaultCursor()
{
    BaseInteraction* i = qobject_cast<BaseInteraction*>(interaction());
    Q_ASSERT(i);
    setCursor(i->cursor());
}

/*! A basic slot to force repaint() of background and foreground.
  */
void MPMapView::invalidateAll()
{
    invalidate(true, true);
}

/*! Load the document content into the view and populate the layer switcher.
  */
void MPMapView::setDocument(Document* aDoc)
{
    MapView::setDocument(aDoc);
    m_layerswitcher->setDocument(aDoc);
}

/*! When mouse moves.
  */
void MPMapView::mouseMoveEvent(QMouseEvent* event)
{
    MapView::mouseMoveEvent(event);
    emit mouseMove(event);
}

/*! When widget is painted.
  */
void MPMapView::paintEvent(QPaintEvent* event)
{
    QTime Start(QTime::currentTime());

    if (!StaticBufferUpToDate) {
        setCursor(Qt::WaitCursor);  // show only Wait on features paint.
    }
    MapView::paintEvent(event);
    QTime Stop(QTime::currentTime());
    emit painted(Start.msecsTo(Stop));
    updateDefaultCursor();
}

/*! Overriden resizeEvent() in order to place the layer switcher in
  top right corner of the map.
  */
void MPMapView::resizeEvent(QResizeEvent* event)
{
    // Place the layer switcher in top-right corner
    m_layerswitcher->move(this->width() - m_layerswitcher->width(), 0);
    MapView::resizeEvent(event);
}

/*! When a layer requests an image.
  */
void MPMapView::on_imageRequested(ImageMapLayer* aLayer)
{
    MapView::on_imageRequested(aLayer);
    ++m_numImages;
    emit imageRequested(m_numImages);
}

/*! When a layer receives an image.
  */
void MPMapView::on_imageReceived(ImageMapLayer* aLayer)
{
    MapView::on_imageReceived(aLayer);
    emit imageReceived();
}

/*! When a layer has finished loading its images.
  */
void MPMapView::on_loadingFinished(ImageMapLayer* aLayer)
{
    MapView::on_loadingFinished(aLayer);
    emit imageFinished();
    m_numImages = 0;
}

/*! Default interaction instance.

  \returns A pointer to a new instance of Interaction.
  */
Interaction *MPMapView::defaultInteraction()
{
    return new BaseInteraction(this);
}

/*! Start an interaction
  \param anInteraction A pointer to an Interaction
  */
void MPMapView::launch(Interaction* anInteraction)
{
    MapView::launch(anInteraction);
    BaseInteraction* interaction = dynamic_cast<BaseInteraction*>(anInteraction);

    if (interaction) {
        connect(interaction, SIGNAL(idle()), this, SLOT(on_userIdle()));
        connect(interaction, SIGNAL(featureSnap(Feature*)), this, SLOT(on_featureSnap(Feature*)));
    }
}

/*! When a feature is snapped (hovered)
  \see signal Interaction::featureSnap(Feature* feature)
 */
void MPMapView::on_featureSnap(Feature* feature)
{
    if (feature) {
        BaseInteraction* i = qobject_cast<BaseInteraction*>(interaction());
        if (i->isSnapEnabled()) {
            setCursor(Qt::ArrowCursor);
        }
        setToolTip(feature->toHtml());
    }
    else {
        updateDefaultCursor();
        setToolTip("");
    }
    emit featureSnap(feature);
}

/*! When the user is idle (inactive)

  Detects if the previous viewport is very different, and if so
  emits viewportShift().
  \see signal BaseInteraction::idle()
 */
void MPMapView::on_userIdle()
{
    // Detect big moves in viewport
    if (!m_previousviewport.isNull()) {
        QRectF intersect = viewport().intersected(m_previousviewport);
        qreal intersurface = qAbs(intersect.height()) * qAbs(intersect.width());
        qreal surface = qAbs(viewport().latDiff()) * qAbs(viewport().lonDiff());

        // If only half of the previous viewport is now visible
        if ((intersurface < surface) &&
            (intersurface / surface <= VIEWPORT_SHIFT_PERCENT)) {
            emit viewportShift();
        }
    }
    m_previousviewport = viewport();
}
