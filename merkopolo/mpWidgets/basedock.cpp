#include "basedock.h"

/*! \class BaseDock
    \brief A very basic dock.
  */

/*! \fn void BaseDock::dockClosed(bool)
  This signal is emitted when the dock is closed (or hidden). This distinguish the close event
  from the visibility event, triggered when the dock is placed in a tab widget.
*/

/*! Constructs a BaseDock
  */
BaseDock::BaseDock(QWidget* parent) :
    MDockAncestor(parent)
{
}

/*! Destroys a BaseDock
  */
BaseDock::~BaseDock()
{
}

/*! Re-implementation of closeEvent
  */
void BaseDock::closeEvent(QCloseEvent * event)
{
    Q_UNUSED(event);
    emit dockClosed(false);
}
