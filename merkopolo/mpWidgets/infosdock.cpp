#include "infosdock.h"

#include "mpwindow.h"

/*! \class InfosDock
    \brief A dock to display informations on hovered features.
  */

/*! Constructs a InfosDock
  */
InfosDock::InfosDock(QWidget *parent) :
    BaseDock(parent),
    m_text(new QTextBrowser(this))
{
    setMinimumSize(220,100);
    setWindowTitle(tr("Info"));
    setObjectName("infosDock");

    m_text->setReadOnly(true);
    m_text->setOpenLinks(false);
    setWidget(m_text);
}

/*! Destroys the InfosDock
  */
InfosDock::~InfosDock(void)
{
    delete m_text;
}

/*! Update the current HTML content of the dock.
  */
void InfosDock::setHtml(QString html)
{
    m_currentHtml = html;
    m_text->setHtml(html);
}

/*! Temporary HTML content (will not override current content).
  */
void InfosDock::setHoverHtml(QString html)
{
    m_text->setHtml(html);
}

/*! Remove hover content and show current content.
  */
void InfosDock::unsetHoverHtml()
{
    m_text->setHtml(m_currentHtml);
}

/*! Return the current HTML content.
  */
QString InfosDock::getHtml()
{
    return m_text->toHtml();
}


