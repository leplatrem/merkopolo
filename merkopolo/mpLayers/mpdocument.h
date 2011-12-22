#ifndef MPDOCUMENT_H
#define MPDOCUMENT_H

#include "Document.h"

#include "mpfeaturepainter.h"

class MPDocument : public Document
{
public:
    MPDocument();
    void setPainters(QList<Painter>);
    int getPaintersSize();
    const Painter* getPainter(int);
    void moveLayer(Layer*, int);

protected:
    /*! Protected list of painters (like private list in parent class). */
    QList<MPFeaturePainter> m_painters;
};

#endif // MPDOCUMENT_H
