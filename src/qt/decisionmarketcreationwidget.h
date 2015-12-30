#ifndef DECISIONMARKETCREATIONWIDGET_H
#define DECISIONMARKETCREATIONWIDGET_H

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

private:
    Ui::DecisionMarketCreationWidget *ui;
    DecisionDecisionWindow *decisionWindow;
};

#endif // DECISIONMARKETCREATIONWIDGET_H
