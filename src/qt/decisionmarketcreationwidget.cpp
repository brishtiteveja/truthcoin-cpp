#include "decisionmarketcreationwidget.h"
#include "ui_decisionmarketcreationwidget.h"

#include "decisiondecisionwindow.h"
#include "json/json_spirit_writer_template.h"

#include <QMessageBox>

DecisionMarketCreationWidget::DecisionMarketCreationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionMarketCreationWidget)
{
    ui->setupUi(this);

    ui->pushButtonUpdateMarket->hide();

    updateIndex = -1;

    connect(this, SIGNAL(inputError(QString)),
            this, SLOT(on_inputError(QString)));
}

DecisionMarketCreationWidget::~DecisionMarketCreationWidget()
{
    delete ui;
}

json_spirit::Array DecisionMarketCreationWidget::createDecisionMarketArray()
{
    // Grab user input from the ui
    QString address = ui->lineEditAuthorAddress->text();
    QString decisionID = ui->lineEditDecisions->text();
    decisionID.append(":");
    decisionID.append(ui->comboBoxFunctions->currentText());
    double B = ui->doubleSpinBoxB->value();
    double tradingFee = ui->doubleSpinBoxTradeFee->value();
    double maxCommission = ui->doubleSpinBoxMaxCommission->value();
    QString title = ui->lineEditTitle->text();
    QString description = ui->plainTextEditDescription->toPlainText();
    QString tags = ui->lineEditTags->text();
    int maturation = 1;
    int txPoWh = ui->comboBoxHashFunction->currentIndex();
    int txPoWd = ui->doubleSpinBoxDifficulty->value();

    bool error = false;

    // Perform basic checks and avoid wasting json calls
    if (address.isEmpty()) {
        emit inputError("You must enter a valid Hivemind address!");
        error = true;
    }

    if (decisionID.isEmpty()) {
        emit inputError("You must enter a decision ID!");
        error = true;
    }

    if (B == 0) {
        emit inputError("You must enter a valid B value!");
        error = true;
    }

    if (tradingFee == 0) {
        emit inputError("You must enter a valid trading fee!");
        error = true;
    }

    if (maxCommission == 0) {
        emit inputError("You must enter a valid max commission!");
        error = true;
    }

    if (title.isEmpty()) {
        emit inputError("You must enter a valid title!");
        error = true;
    }

    if (description.isEmpty()) {
        emit inputError("You must enter a valid Hivemind description!");
        error = true;
    }

    if (tags.isEmpty()) {
        emit inputError("You must enter valid tags!");
        error = true;
    }

    if (maturation == 0) {
        emit inputError("You must enter a valid maturation date!");
        error = true;
    }

    if (txPoWd == 0) {
        emit inputError("You must enter a valid PoW difficulty!");
        error = true;
    }

    // Setup json_spirit array
    json_spirit::Array params;

    // Return empty array if there was an input error
    if (error) return params;

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

    return params;
}

void DecisionMarketCreationWidget::on_pushButtonCreateMarket_clicked()
{
    json_spirit::Array decisionMarketArray = createDecisionMarketArray();
    if (!decisionMarketArray.empty()) {
        emit newDecisionMarketArray(decisionMarketArray);
    }
}

void DecisionMarketCreationWidget::on_pushButtonSelectDecision_clicked()
{
    decisionWindow = new DecisionDecisionWindow(this);
    decisionWindow->show();
    decisionWindow->raise();
    decisionWindow->setFocus();
}

void DecisionMarketCreationWidget::on_inputError(const QString &errorMessage)
{
    QMessageBox errorMessageBox;
    errorMessageBox.setWindowTitle("Input error!");
    errorMessageBox.setText(errorMessage);
    errorMessageBox.setStandardButtons(QMessageBox::Ok);
    errorMessageBox.setDefaultButton(QMessageBox::Ok);
    errorMessageBox.exec();
}

void DecisionMarketCreationWidget::editArray(json_spirit::Array array)
{
    if (array.size() < 12) {
        return;
    }

    // Load address
    json_spirit::Value address = array.at(0);
    ui->lineEditAuthorAddress->setText(QString::fromStdString(address.get_str()));

    // Load decisionID & Function option
    json_spirit::Value decisionID = array.at(1);
    ui->lineEditDecisions->setText(QString::fromStdString(decisionID.get_str()));

    // Load B
    json_spirit::Value B = array.at(2);
    ui->doubleSpinBoxB->setValue(B.get_real());

    // Load tradingFee
    json_spirit::Value tradingFee = array.at(3);
    ui->doubleSpinBoxTradeFee->setValue(tradingFee.get_real());

    // Load maxCommission
    json_spirit::Value maxCommission = array.at(4);
    ui->doubleSpinBoxMaxCommission->setValue(maxCommission.get_real());

    // Load title
    json_spirit::Value title = array.at(5);
    ui->lineEditTitle->setText(QString::fromStdString(title.get_str()));

    // Load description
    json_spirit::Value description = array.at(6);
    ui->plainTextEditDescription->setPlainText(QString::fromStdString(description.get_str()));

    // Load tags
    json_spirit::Value tags = array.at(7);
    ui->lineEditTags->setText(QString::fromStdString(tags.get_str()));

    // Load maturation
    json_spirit::Value maturation = array.at(8);
    ui->spinBoxMaturation->setValue(maturation.get_int());

    // Load txPoWh
    json_spirit::Value txPoWh = array.at(9);
    ui->comboBoxHashFunction->setCurrentIndex(txPoWh.get_int());

    // Load txPoWd
    json_spirit::Value txPoWd = array.at(10);
    ui->doubleSpinBoxDifficulty->setValue(txPoWd.get_int());

    if (array.size() == 13) {
        // Show update button and set index
        ui->pushButtonCreateMarket->hide();
        ui->pushButtonUpdateMarket->show();
        updateIndex = array.at(12).get_int();
    }
}

void DecisionMarketCreationWidget::on_pushButtonUpdateMarket_clicked()
{
    json_spirit::Array updatedMarket = createDecisionMarketArray();
    if (!updatedMarket.empty() && updateIndex >= 0) {
        updatedMarket.push_back(updateIndex);
        emit updatedDecisionMarketArray(updatedMarket);
    }
}
