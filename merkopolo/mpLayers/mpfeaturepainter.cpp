#include "mpfeaturepainter.h"

/*!
  \class MPFeaturePainter
  \brief A custom feature painter to control direction arrows drawing.
 */

/*! Constructs a feature painter.
  /note useless ?
  */
MPFeaturePainter::MPFeaturePainter()
    : FeaturePainter()
{
}

/*! Constructs a feature painter from a painter
  instantiated in IPaintStyle.
  */
MPFeaturePainter::MPFeaturePainter(const Painter& f) :
    FeaturePainter(f)
{
}
