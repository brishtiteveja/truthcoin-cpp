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
    QString decisionID = "";
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

    extern json_spirit::Value createmarket(const json_spirit::Array &params, bool fHelp);

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

    // Create market, passing spirit array and returning spirit object
    json_spirit::Value result;
    try {
        result = createmarket(params, false);
    } catch (const std::runtime_error &error) {
        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
        std::cout << "Error: \n" << error.what() << std::endl;
        return;
    } catch (const std::exception &exception) {
        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
        std::cout << "Exception: \n" << exception.what() << std::endl;
        return;
    }  catch (const json_spirit::Object &object) {
        result = object;
    } catch (...) {
        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
        std::cout << "Unknown Exception!\n";
        return;
    }

    // Unpack spirit results
    try {
        std::string text = json_spirit::write_string(result, true);
        std::cout << "Create Market Result: " << text << std::endl;
    } catch (...) {
        std::cout << "decisioncreationwidget::on_pushButtonCreateMarket clicked\n";
        std::cout << "write_string: Unknown Exception!\n";
    }
}

void DecisionMarketCreationWidget::on_pushButtonSelectDecision_clicked()
{
    decisionWindow = new DecisionDecisionWindow(this);
    decisionWindow->show();
    decisionWindow->raise();
    decisionWindow->setFocus();
}
