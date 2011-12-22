#include "coordfield.h"

/*! \class CoordField
    \brief A widget which create a simple QLineEdit field for displaying coordinates.
    \brief User may also enter some coordinates and center the map on it.
  */

/*! \fn void CoordField::centerView(qreal, qreal)
  This signal is emitted for recenter the view (x,y)
*/

/*! Constructs a CoordField
  */
CoordField::CoordField(QWidget* parent) :
    QLineEdit(parent)
{
    setCoord(QPointF(0, 0));
    setMinimumWidth(100);
    setMaximumWidth(150);
    setAlignment(Qt::AlignHCenter);
    connect(this, SIGNAL(returnPressed()), this, SLOT(coordsSet()));
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
}

/*! Destroys the CoordField
  */
CoordField::~CoordField()
{
}

/*! Some coords have been entered by the user, and he pressed enter
   */
void CoordField::coordsSet() {
    QString coords = text();
    qreal x,y;
    if(!verifCoords(coords, x, y))
        return;
    emit centerView(x,y);
}

/*! The user is entering new coordinates
   */
void CoordField::textEdited(QString coords) {
    qreal x,y;
    if(!verifCoords(coords,x,y))
        setError(true);
    else
        setError(false);
}

/*! The user is entering new coordinates
   */
bool CoordField::verifCoords(QString coords, qreal& x , qreal& y) {
    QStringList coordList = coords.split(",");
    if(coordList.size() != 2)
        return false;
    bool xOk, yOk;
    x = QString(coordList.at(0)).toDouble(&xOk);
    y = QString(coordList.at(1)).toDouble(&yOk);
    if(!xOk || !yOk || x < -180 || x > 180 || y < -90 || y > 90)
        return false;
    return true;
}

/*! Indicate error
   */
void CoordField::setError(bool error) {
    QString style;
    if(error)
        style = "color:red;";
    setStyleSheet(style);
}

/*! Fill the lineEdit with coords
   */
void CoordField::setCoord(QPointF position) {
    setError(false);
    // Give feedback on current viewport
    QString vpLabel = QString("%1 , %2").arg(position.x(),0,'f',4).arg(position.y(),0,'f',4);
    setText(vpLabel);
}
