#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QMessageBox>

#include "ImageMapLayer.h"
#include "Layer.h"
#include "MasPaintStyle.h"
#include "Interaction.h"

#include "mpglobal.h"
#include "mpwindow.h"
#include "ui_mpwindow.h"
#include "mpmapview.h"
#include "mpdocument.h"
#include "baseinteraction.h"
#include "infosdock.h"
#include "zoomregioninteraction.h"
#include "coordfield.h"


/*!
  \class MPWindow
  \brief The main application window.

  This class contains the main logics of the application.
  All components instantiation and wiring is done here.
*/

/*! \fn void MPWindow::close()
    This signal is emitted when this window is closed.
*/

/*! Constructs the main application window.

  Initialize all components and connect them.
  */
MPWindow::MPWindow(QWidget *parent) :
    QMainWindow(parent),
    m_view(0),
    m_document(0),
    m_streetlayer(0),
    m_infosdock(0),
    m_coordsLabel(0),
    m_paintTimeLabel(0),
    m_meterPerPixelLabel(0),
    m_imagesProgress(0),
    m_dataProgress(0),
    m_wsProgress(0),
    ui(new Ui::MPWindow)
{
    ui->setupUi(this);
    setWindowTitle(windowTitle() + QString(" - %1").arg(VERSION));

    m_streetlayer = new ImageMapLayer("");
    m_streetlayer->setMapAdapter(TMS_ADAPTER_UUID, "OSM Mapnik");
    m_streetlayer->setVisible(true);

    initUIComponents();

    // Allow slots connecting between threads with type CoordBox
    qRegisterMetaType<CoordBox>();

    // Restore settings
    loadSettings();

    // Abstract document
    loadDocument(new MPDocument());

    // Default interaction
    m_view->launch(m_view->defaultInteraction());

}

/*! Destroys MPWindow.
  */
MPWindow::~MPWindow()
{
    delete ui;

    delete m_document;  // will delete layers
    delete m_view;
    delete m_infosdock;
    delete m_coordsLabel;
    delete m_meterPerPixelLabel;
    delete m_zoomlevelLabel;
    delete m_paintTimeLabel;
    delete m_imagesProgress;
    delete m_dataProgress;
    delete m_wsProgress;
    delete m_sepCoordZoom;
    delete m_sepZoomScale;
    delete m_sepScaleTime;
}

/*! Implements a clean application exit.
  */
void MPWindow::closeEvent(QCloseEvent *event) {
    emit close();
    saveSettings();
    event->accept();
    qApp->quit();
}

/*! Constructs widgets and connections of signals for the main window.
 */
void MPWindow::initUIComponents()
{
    // Initialise view
    m_view = new MPMapView(this);
    connect(m_view, SIGNAL(interactionChanged(Interaction*)), this, SLOT(onInteractionChanged(Interaction*)));
    connect(m_view, SIGNAL(viewportShift()), this, SLOT(onViewShift()));
    connect(m_view, SIGNAL(painted(qlonglong)), this, SLOT(onViewPainted(qlonglong)));
    connect(m_view, SIGNAL(imageRequested(int)), this, SLOT(onViewImageRequested(int)));
    connect(m_view, SIGNAL(imageReceived()), this, SLOT(onViewImageReceived()));
    connect(m_view, SIGNAL(imageFinished()), this, SLOT(onViewImageFinished()));
    connect(m_view, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onViewMouseMove(QMouseEvent*)));
    connect(m_view, SIGNAL(featureSnap(Feature*)), this, SLOT(onViewFeatureSnap(Feature*)));

    setCentralWidget(m_view);

    // Docks
    m_infosdock = new InfosDock(this);
    m_infosdock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, m_infosdock);
    connect(m_infosdock, SIGNAL(dockClosed(bool)), this, SLOT(onDisplayInfosDock(bool)));

    // Status bar
    m_coordsLabel = new CoordField(this);
    connect(m_coordsLabel, SIGNAL(centerView(qreal,qreal)), this, SLOT(onCenterView(qreal,qreal)));

    m_meterPerPixelLabel = new QLabel(this);
    m_zoomlevelLabel = new QLabel(this);    
    m_paintTimeLabel = new QLabel(this);
    m_paintTimeLabel->setMinimumWidth(23);

    m_dataProgress = new QProgressBar(this);
    m_dataProgress->setMaximumWidth(200);
    m_dataProgress->setVisible(false);

    m_imagesProgress = new QProgressBar(this);
    m_imagesProgress->setMaximumWidth(200);
    m_imagesProgress->setFormat(tr("tile %v / %m"));

    m_wsProgress = new QProgressBar(this);
    m_wsProgress->setMaximumWidth(200);
    m_wsProgress->setTextVisible(true);
    m_wsProgress->setFormat(tr("Requesting...")); // This has no effects, because max = 0
    m_wsProgress->setVisible(false);
    m_wsProgress->setMaximum(0);

    // Separators
    m_sepCoordZoom = new QFrame(this);
    m_sepCoordZoom->setFrameStyle(QFrame::VLine);
    m_sepZoomScale = new QFrame(this);
    m_sepZoomScale->setFrameStyle(QFrame::VLine);
    m_sepScaleTime = new QFrame(this);
    m_sepScaleTime->setFrameStyle(QFrame::VLine);

    statusBar()->addPermanentWidget(m_imagesProgress);
    statusBar()->addPermanentWidget(m_dataProgress);
    statusBar()->addPermanentWidget(m_wsProgress);
    statusBar()->addPermanentWidget(m_coordsLabel);
    statusBar()->addPermanentWidget(m_sepCoordZoom);
    statusBar()->addPermanentWidget(m_zoomlevelLabel);
    statusBar()->addPermanentWidget(m_sepZoomScale);
    statusBar()->addPermanentWidget(m_meterPerPixelLabel);
    statusBar()->addPermanentWidget(m_sepScaleTime);
    statusBar()->addPermanentWidget(m_paintTimeLabel);
}

/*! Load application settings and apply them.
  */
void MPWindow::loadSettings()
{
    QSettings settings;
    ui->displayInfosDockAction->setChecked(settings.value("user/infosdock", false).toBool());
    // Restore application geometry and state (docks toolbars etc.) from settings
    restoreGeometry(settings.value("window/geometry").toByteArray());
    restoreState(settings.value("window/state").toByteArray(), UI_VERSION);
}

/*! Save application settings.
  */
void MPWindow::saveSettings()
{
    QSettings settings;
    // Save application geometry and state to settings for now
    settings.beginGroup("user");
    settings.setValue("infosdock", m_infosdock->isVisible());
    settings.endGroup();

    settings.beginGroup("window");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState(UI_VERSION));
    settings.endGroup();
}

/*! Invalidates the map view completely to refresh content (background and vectorial).
  */
void MPWindow::invalidateView()
{
    m_view->invalidate(true, true);
}

/*! When the current interaction changes.
  */
void MPWindow::onInteractionChanged(Interaction* interaction)
{
    BaseInteraction* baseinteraction = static_cast<BaseInteraction*>(interaction);
    connect(this, SIGNAL(interactionReinitialize()), baseinteraction, SLOT(reinitialize()));
}

/*! When the map view was moved sufficiently to require data reload.
  \see MPMapView::viewportShift()
  */
void MPWindow::onViewShift()
{
    qDebug() << tr("View shift");
}

/*! When mouse is moved on map view
  */
void MPWindow::onViewMouseMove(QMouseEvent* event)
{
    Coord position = m_view->fromView(event->pos());
    m_coordsLabel->setCoord(position);
}

/*! When a feature is snapped (hovered) on map view
  */
void MPWindow::onViewFeatureSnap(Feature* feature)
{
    if (feature) {
        m_infosdock->setHoverHtml(feature->toHtml());
    }
}

/*! When the map view is painted (refreshed)
  */
void MPWindow::onViewPainted(qlonglong elapsed)
{
    m_meterPerPixelLabel->setText(tr("%1m/pixel").arg(1/m_view->pixelPerM(), 0, 'f', 2));
    m_zoomlevelLabel->setText(tr("zoom %1").arg(m_streetlayer->getCurrentZoom()));
    m_paintTimeLabel->setText(tr("%1ms").arg(elapsed));
}

/*! When the \a InfosDock is hidden/shown.
  */
void MPWindow::onDisplayInfosDock(bool state)
{
    ui->displayInfosDockAction->setChecked(state);
}

/*! When the zoom in action is triggered (button)
  \see QMetaObject::connectSlotsByName()
  */
void MPWindow::viewZoomIn()
{
    m_view->zoomIn();
}

/*! When the zoom out action is triggered (button)
  \see QMetaObject::connectSlotsByName()
  */
void MPWindow::viewZoomOut()
{
    m_view->zoomOut();
}

/*! When the zoom fit action is triggered (button)
  \see QMetaObject::connectSlotsByName()
  */
void MPWindow::viewZoomWindow()
{
    m_view->launch(new ZoomRegionInteraction(m_view));
}

/*! When the map view requests an image (tile) download.
  */
void MPWindow::onViewImageRequested(int nbrequested)
{
    m_imagesProgress->setVisible(true);
    m_imagesProgress->setRange(0, nbrequested);
    m_imagesProgress->update();
    if (m_imagesProgress->value() < 0)
        m_imagesProgress->setValue(0);
}

/*! When the map view received an image (tile).
  */
void MPWindow::onViewImageReceived()
{
    int current = m_imagesProgress->value();
    if (current < m_imagesProgress->maximum())
        m_imagesProgress->setValue(current+1);
}

/*! When the map view received all its images (tiles).
  */
void MPWindow::onViewImageFinished()
{
    m_imagesProgress->setVisible(false);
    m_imagesProgress->reset();
}

/*! Explicitly load a document (group of layers).

  Deletes the current one, and replaces it with the one specified.

  \param aDoc A pointer to Document.
  */
void MPWindow::loadDocument(MPDocument *aDoc)
{
    delete m_document;
    m_document = aDoc;

    m_document->addImageLayer(m_streetlayer);
    m_view->setDocument(m_document);
    m_view->projection().setProjectionType(m_streetlayer->projection());

    // Default map position
    Coord toulouse(1.39,43.63);
    m_view->setViewport(CoordBox(toulouse, toulouse), m_view->rect());
    m_coordsLabel->setCoord(toulouse);
}

/*! Show a message popup for critical errors.
  */
void MPWindow::showCriticalError(QString message)
{
    QMessageBox::critical(this,
                          tr("Critical error"),
                          message);
}

/*! Show a status message for warning errors.
  */
void MPWindow::showWarningError(QString message)
{
    if (!message.trimmed().isEmpty())
        statusBar()->showMessage(message);
}


/*! Menu display info activated
  */
void MPWindow::displayInfosDock()
{
    bool state = ui->displayInfosDockAction->isChecked();
    if (state)
        m_infosdock->show();
    else
        m_infosdock->hide();
}

/*! Center the view
   */
void MPWindow::onCenterView(qreal x, qreal y) {
    Coord center(x,y);
    m_view->setCenter(center,m_view->rect());
    invalidateView();
}
