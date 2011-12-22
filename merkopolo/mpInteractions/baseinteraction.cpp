#include "baseinteraction.h"

#include "Layer.h"

#include "mpmapview.h"


/*!
  \class BaseInteraction
  \brief A base interaction class to be overriden for more specific actions.

  This class inherits the common set of actions that will always be enabled
  in the application.

  \li mouse panning
  \li arrows keyboard panning
  \li zoom wheel
  \li +/- keyboard zoom
  \li zoom box (Ctrl)
  \li feature snapping (if enabled)
*/

/*! \fn void BaseInteraction::idle();
    This signal is emitted when the user is inactive for some time (IDLE_TIMEOUT)
*/

/*! Constructs BaseInteraction
  */
BaseInteraction::BaseInteraction(MPMapView* theView) :
    FeatureSnapInteraction(theView),
    m_snapEnabled(true)
{
    m_idletimer = new QTimer(this);
    m_idletimer->setInterval(IDLE_TIMEOUT);
    m_idletimer->setSingleShot(true);
    connect(m_idletimer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    setDontSelectVirtual(true);
}

/*! Destroys BaseInteraction
  */
BaseInteraction::~BaseInteraction(void)
{
    delete m_idletimer;
}

/*! Reinitializes the interaction state. Basically used
  when loading another workspace.
  */
void BaseInteraction::reinitialize()
{
}

/*! Enables snap (hovering) of features.
  */
void BaseInteraction::setSnapEnabled(bool enabled)
{
    m_snapEnabled = enabled;
}

/*! Checks whether snap (hovering) of features is enabled.
  */
bool BaseInteraction::isSnapEnabled()
{
    return m_snapEnabled;
}

/*! Current interaction HTML documentation (to be displayed in help panel).
  */
QString BaseInteraction::toHtml()
{
    return QString();
}

/*! When a mouse button is pressed.
  \warning Different from clic.
  */
void BaseInteraction::mousePressEvent(QMouseEvent* event)
{
    FeatureSnapInteraction::mousePressEvent(event);
    Dragging = false; // Disable zoom drag

    // Do not fire idle() while panning or dragging
    if (Panning)
        m_idletimer->stop();
}

/*! When a mouse button is released.
  */
void BaseInteraction::mouseReleaseEvent(QMouseEvent* event)
{
    FeatureSnapInteraction::mouseReleaseEvent(event);
    if (!m_idletimer->isActive())
        resetIdleTimer();
}

/*! When the mouse is moved.
  */
void BaseInteraction::mouseMoveEvent(QMouseEvent* event)
{
    snapMouseMoveEvent(event, LastSnap);
    if (!LastSnap)
        Interaction::mouseMoveEvent(event);
    if (!(Panning))
        resetIdleTimer();
}

/*! When the mouse wheel is rolled.
  */
void BaseInteraction::wheelEvent(QWheelEvent* event)
{
    Interaction::wheelEvent(event);
    resetIdleTimer();
}

/*! When a double clic is triggered.
  */
void BaseInteraction::mouseDoubleClickEvent(QMouseEvent* event)
{
    FeatureSnapInteraction::mouseDoubleClickEvent(event);
    resetIdleTimer();
}

/*! Resets the idle timer.

  Make a call to this function basically when the user does something.
  */
void BaseInteraction::resetIdleTimer()
{
    // User has done something, restart idle timer
    m_idletimer->start();
}

/*! The user has not done anything since last idle timer reset.

    Emits BaseInteraction::idle()
  */
void BaseInteraction::onTimerTimeout()
{
    emit idle();
    m_idletimer->stop();
}

/*! Paints interaction on map view (draw hovered features etc.)

  In Merkaartor, FeatureSnapInteraction::paintEvent() has dependencies
  to Merkaartor main window. Here we bypass and overide Interaction::paintEvent().
  */
void BaseInteraction::paintEvent(QPaintEvent* anEvent, QPainter& thePainter)
{
    Interaction::paintEvent(anEvent, thePainter);
    if (LastSnap) {
        LastSnap->drawHover(thePainter, view());
    }
}

/*! Detects snapped features on mouse move (if snapping is enabled).
  */
void BaseInteraction::updateSnap(QMouseEvent* event)
{
    if (!isSnapEnabled())
        return;

    FeatureSnapInteraction::updateSnap(event);
}
