#include "decisionmarketcreationwidget.h"
#include "ui_decisionmarketcreationwidget.h"

#include "decisiondecisionwindow.h"
#include "json/json_spirit_writer_template.h"
#include "primitives/market.h"

DecisionMarketCreationWidget::DecisionMarketCreationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionMarketCreationWidget)
{
    ui->setupUi(this);
}

DecisionMarketCreationWidget::~DecisionMarketCreationWidget()
{
    delete ui;
}

void DecisionMarketCreationWidget::on_pushButtonCreateMarket_clicked()
{
    // Grab user input from the ui
    QString address = ui->lineEditAuthorAddress->text();
    QString decisionID = ui->lineEditDecisions->text();
    decisionID.append(":");
    decisionID.append(ui->lineEditFunctions->text());
    double B = ui->lineEditB->text().toDouble();
    double tradingFee = ui->doubleSpinBoxTradeFee->value();
    double maxCommission = ui->doubleSpinBoxMaxCommission->value();
    QString title = ui->lineEditTitle->text();
    QString description = ui->plainTextEditDescription->toPlainText();
    QString tags = ui->lineEditTags->text();
    int maturation = 1;
    int txPoWh = ui->comboBoxHashFunction->currentIndex();
    int txPoWd = ui->doubleSpinBoxDifficulty->value();

    // Setup json_spirit array
    json_spirit::Array params;
    params.push_back(address.toStdString());
    params.push_back(decisionID.toStdString());
    params.push_back(B);
    params.push_back(tradingFee);
    params.push_back(maxCommission);
    params.push_back(title.toStdString());
    params.push_back(description.toStdString());
    params.push_back(tags.toStdString());
    params.push_back(maturation);
    params.push_back(txPoWh);
    params.push_back(txPoWd);
    // Array type
    params.push_back("market");

    emit receivedDecisionMarketArray(params);
}

void DecisionMarketCreationWidget::on_pushButtonSelectDecision_clicked()
{
    decisionWindow = new DecisionDecisionWindow(this);
    decisionWindow->show();
    decisionWindow->raise();
    decisionWindow->setFocus();
}
