#ifndef DECISIONMARKETCREATIONWIDGET_H
#define DECISIONMARKETCREATIONWIDGET_H

#include "json/json_spirit_writer_template.h"

#include <QWidget>

class DecisionDecisionWindow;

namespace Ui {
class DecisionMarketCreationWidget;
}

class DecisionMarketCreationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionMarketCreationWidget(QWidget *parent = 0);
    ~DecisionMarketCreationWidget();

private slots:
    void on_pushButtonCreateMarket_clicked();

    void on_pushButtonSelectDecision_clicked();

signals:
    /** Signal raised when json_spirit::Array for new Decision Market is created */
    void receivedDecisionMarketArray(const json_spirit::Array array);

private:
    Ui::DecisionMarketCreationWidget *ui;
    DecisionDecisionWindow *decisionWindow;
};

#endif // DECISIONMARKETCREATIONWIDGET_H
