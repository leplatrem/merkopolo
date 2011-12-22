#include "layerswitcher.h"
#include "ui_layerswitcher.h"

#include <QHBoxLayout>

#include "Document.h"
#include "Layer.h"


/*! \class SwitchButton
    \brief A button to switch the visibility of its associated \a Layer.
  */

/*! \fn void SwitchButton::toggled(bool, Layer*)
  This signal is emitted when the button is toggled.
  */
/*! \fn void SwitchButton::checked(bool, Layer*)
  This signal is emitted when the button is checked.
  */

/*! Constructs a SwitchButton
  */
SwitchButton::SwitchButton(Layer *layer, QWidget *parent) :
    QPushButton(parent),
    m_layer(layer),
    m_selectableLayer(NULL)
{
    if(!layer->isReadonly()) {
        // Add a checkbox to enable/disable select on the layer
        m_selectableLayer = new QCheckBox(this);
        m_selectableLayer->setStyleSheet("margin-top:4px; margin-left:4px");
        m_selectableLayer->setChecked(true);
        connect(m_selectableLayer, SIGNAL(toggled(bool)), this, SLOT(onChecked(bool)));
    }

    setCheckable(true);
    setChecked(layer->isVisible());
    onToggle(layer->isVisible());
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onToggle(bool)));

    // Set a small font and reduce button margins
    QFont f = QFont(font());
    f.setPointSize(9);
    setFont(f);
    setCursor(Qt::ArrowCursor);

    setMinimumHeight(fontMetrics().boundingRect(text()).height() + 12);
    setMaximumHeight(fontMetrics().boundingRect(text()).height() + 12);
}

/*! Refreshes the button apparence. This basically means to update the
    font according to the button state and reload the color from the layer.
  */
void SwitchButton::refresh()
{
    QString style = "QWidget { font-weight: %2 }";
    if (m_selectableLayer) {
        style = "QWidget { text-align: right; padding-right: 5px; font-weight: %2 }";
        m_selectableLayer->setEnabled(isChecked());
    }
    setStyleSheet(style.arg(isChecked() ? "bold" : "normal"));
    setChecked(m_layer->isVisible());
    setText(m_layer->name());
    setMinimumWidth(fontMetrics().boundingRect(text()).width() + 30);
    setMaximumWidth(fontMetrics().boundingRect(text()).width() + 30);
}

/*! Re-emit the toggle() signal along with the associated layer pointer.
  */
void SwitchButton::onToggle(bool state)
{
    emit toggled(state, m_layer);
    refresh();
}

/*! Re-emit the checked() signal along with the associated layer pointer.
  */
void SwitchButton::onChecked(bool state)
{
    emit checked(state, m_layer);
}


/*! \class LayerSwitcher
    \brief A widget to switch the layers of its associated \a Document.
  */

/*! \fn void LayerSwitcher::layerSwitched()
  This signal is emitted when a layer's SwitchButton is toggled.
  */
/*! \fn void LayerSwitcher::layerChecked()
  This signal is emitted when a layer's SwitchButton is checked.
  */

/*! Constructs a LayerSwitcher
  */
LayerSwitcher::LayerSwitcher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerSwitcher)
{
    ui->setupUi(this);
    this->setLayout(new QHBoxLayout());
}

/*! Destroys a LayerSwitcher
  */
LayerSwitcher::~LayerSwitcher()
{
    delete ui;
}

/*! Associate a \a Document to this layer switcher.

  A SwitchButton will be added for each layer of the Document (layers with no name will be ignored).
  */
void LayerSwitcher::setDocument(Document* doc)
{
    SwitchButton* btn = NULL;
    for (int i=0; i < doc->layerSize(); i++) {
        Layer* l = doc->getLayer(i);
        if (l->name().isEmpty())
            continue;   // Do not show buttons for unnamed layers
        btn = new SwitchButton(l, this);
        connect(btn, SIGNAL(toggled(bool, Layer*)), this, SLOT(switchToggled(bool, Layer*)));
        connect(btn, SIGNAL(checked(bool, Layer*)), this, SLOT(switchChecked(bool, Layer*)));
        layout()->addWidget(btn);
    }
    this->adjustSize();
}

/*! Refreshes all the layer switcher buttons.
  */
void LayerSwitcher::refreshButtons()
{
    for(int i=0; i<layout()->count(); i++)
    {
        QWidget* w = layout()->itemAt(i)->widget();
        SwitchButton* btn = dynamic_cast<SwitchButton*>(w);
        if (btn) btn->refresh();
    }
    adjustSize();
}

/*! When a SwitchButton is toggled.
  */
void LayerSwitcher::switchToggled(bool state, Layer* l)
{
    l->setVisible(state);
    emit layerSwitched();
}

/*! When a SwitchButton's checkbox is checked.
  */
void LayerSwitcher::switchChecked(bool state, Layer* l)
{
    l->setReadonly(!state);
    emit layerChecked();
}
