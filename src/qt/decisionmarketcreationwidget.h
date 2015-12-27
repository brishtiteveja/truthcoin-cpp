#ifndef DECISIONMARKETCREATIONWIDGET_H
#define DECISIONMARKETCREATIONWIDGET_H

#include <QWidget>

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

private:
    Ui::DecisionMarketCreationWidget *ui;
};

#endif // DECISIONMARKETCREATIONWIDGET_H
