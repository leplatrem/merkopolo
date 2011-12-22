#include "mpdocument.h"

#include "MerkaartorPreferences.h"
#include "IPaintStyle.h"


/*!
  \class MPDocument
  \brief A custom document to control instantiation of feature painters (MPFeaturePainter).
 */

/*! Constructs a document.
  This will instantiate as much painters as there are in map style.
  */
MPDocument::MPDocument() :
    Document()
{
    for (int i=0; i<M_STYLE->painterSize(); ++i) {
        m_painters.append(MPFeaturePainter(*M_STYLE->getPainter(i)));
    }
}

/*! Replaces the painters with the specified ones. Used to refresh style.
  Custom feature painters will be instantiated.
  */
void MPDocument::setPainters(QList<Painter> aPainters)
{
    m_painters.clear();
    for (int i=0; i<aPainters.size(); ++i) {
        MPFeaturePainter fp(aPainters[i]);
        m_painters.append(fp);
    }
}

/*! Overriden method to get rid of private parts from parent class Document.
  */
int MPDocument::getPaintersSize()
{
    return m_painters.size();
}

/*! Overriden method to get rid of private parts from parent class Document.
  */
const Painter* MPDocument::getPainter(int i)
{
    return &m_painters[i];
}

/*! Overrides Document::moveLayer(Layer*, int) in order
  to set the level of the specified layer.
  */
void MPDocument::moveLayer(Layer *aLayer, int pos)
{
    for(int i=0; i<aLayer->size(); ++i) {
        Feature* f = aLayer->get(i);
        // This tag is used in Way::updateMeta()
        f->setTag("layer", QString("%1").arg(pos));
    }
    Document::moveLayer(aLayer, pos);
}
