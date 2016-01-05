#ifndef COMBOCREATIONWIDGET_H
#define COMBOCREATIONWIDGET_H

#include "decisioncreationwidget.h"
#include "decisionmarketcreationwidget.h"
#include "json/json_spirit_writer_template.h"

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {
class ComboCreationWidget;
}

class ComboCreationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ComboCreationWidget(QWidget *parent = 0);
    ~ComboCreationWidget();

private:
    Ui::ComboCreationWidget *ui;

    DecisionCreationWidget *decisionCreationWidget;
    DecisionMarketCreationWidget *marketCreationWidget;

signals:
    /** Signal raised when json_spirit::Array for new Combo (Market & Decision) is created */
    void receivedComboArray(const json_spirit::Array &array);
};

#endif // COMBOCREATIONWIDGET_H
