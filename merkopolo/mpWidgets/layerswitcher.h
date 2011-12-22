#ifndef LAYERSWITCHER_H
#define LAYERSWITCHER_H

#include <QWidget>
#include <QHash>
#include <QPushButton>
#include <QCheckBox>

namespace Ui {
    class LayerSwitcher;
}

class Document;
class Layer;


class SwitchButton : public QPushButton
{
    Q_OBJECT

public:
    explicit SwitchButton(Layer* layer, QWidget *parent = 0);

public slots:
    void refresh();

signals:
    void toggled(bool, Layer*);
    void checked(bool, Layer*);

protected slots:
    void onToggle(bool);
    void onChecked(bool);

protected:
    /*! A private pointer to the associated layer. */
    Layer* m_layer;
    /*! A pointer to the selectable layer's checkbox. */
    QCheckBox* m_selectableLayer;
};



class LayerSwitcher : public QWidget
{
    Q_OBJECT

public:
    explicit LayerSwitcher(QWidget *parent = 0);
    ~LayerSwitcher();

signals:
    void layerSwitched();
    void layerChecked();

public slots:
    void refreshButtons();
    void setDocument(Document*);
    void switchToggled(bool, Layer*);
    void switchChecked(bool, Layer*);

private:
    /*! Pointer to UI window form */
    Ui::LayerSwitcher *ui;
};

#endif // LAYERSWITCHER_H
