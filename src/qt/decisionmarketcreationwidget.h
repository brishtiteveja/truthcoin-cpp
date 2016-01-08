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

    void on_inputError(const QString &errorMessage);

    void on_pushButtonUpdateMarket_clicked();

public slots:
    void editArray(json_spirit::Array array);

signals:
    /** Signal raised when json_spirit::Array for new Decision Market is created */
    void newDecisionMarketArray(const json_spirit::Array array);

    /** Signal raised when user updates a decision array */
    void updatedDecisionMarketArray(const json_spirit::Array array);

    /** Signal raised for user input errors on market creation widget */
    void inputError(const QString &error);

private:
    Ui::DecisionMarketCreationWidget *ui;
    json_spirit::Array createDecisionMarketArray();
    DecisionDecisionWindow *decisionWindow;
    int updateIndex;
};

#endif // DECISIONMARKETCREATIONWIDGET_H
