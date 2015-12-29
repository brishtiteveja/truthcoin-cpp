#include "decisioncreationwidget.h"
#include "ui_decisioncreationwidget.h"

#include "primitives/market.h"
#include "json/json_spirit_writer_template.h"

DecisionCreationWidget::DecisionCreationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionCreationWidget)
{
    ui->setupUi(this);
}

DecisionCreationWidget::~DecisionCreationWidget()
{
    delete ui;
}

void DecisionCreationWidget::on_pushButtonCreateDecision_clicked()
{
    // Main branch ID
    QString branchID = "0f894a25c5e0318ee148fe54600ebbf50782f0a1df1eb2aab06321a8ccec270d";

    // Grab user input from ui
    QString address = (ui->lineEditOwnerAddr->text().size())? ui->lineEditOwnerAddr->text() : "<address>";
    QString prompt = (ui->plainTextEditPrompt->toPlainText().size())? ui->plainTextEditPrompt->toPlainText(): "<prompt>";
    int eventOverBy = ui->lineEditEventOverBy->text().toInt();
    bool voteMandatory = ui->checkBoxVoteMandatory->isChecked();
    bool scaled = ui->radioButtonScaled->isChecked();
    double scaledMin = ui->doubleSpinBoxScaledMin->value();
    double scaledMax = ui->doubleSpinBoxScaledMax->value();

    extern json_spirit::Value createdecision(const json_spirit::Array &params, bool fHelp);

    // Setup json_spirit array
    json_spirit::Array params;
    params.push_back(address.toStdString());
    params.push_back(branchID.toStdString());
    params.push_back(prompt.toStdString());
    params.push_back(eventOverBy);
    params.push_back(voteMandatory);
    params.push_back(scaled);
    if (scaled) {
        params.push_back(scaledMin);
        params.push_back(scaledMax);
    }

    // Create decision, passing spirit array and returning spirit object
    json_spirit::Value result;
    try {
        result = createdecision(params, false);
    } catch (const std::runtime_error &error) {
        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
        std::cout << "Error: \n" << error.what() << std::endl;
        return;
    } catch (const std::exception &exception) {
        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
        std::cout << "Exception: \n" << exception.what() << std::endl;
        return;
    } catch (const json_spirit::Object &object) {
        result = object;
    } catch (...) {
        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
        std::cout << "Unknown Exception!\n";;
        return;
    }

    // Unpack spirit results
    try {
        std::string text = json_spirit::write_string(result, true);
        std::cout << "CLI Response: \n" << text << std::endl;
    } catch (...) {
        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
        std::cout << "write_string: Unknown Exception!\n";
    }
}

void DecisionCreationWidget::on_radioButtonBinary_clicked(bool checked)
{
    // Enable scale doublespinbox widgets for binary decision
    if (checked) {
        ui->doubleSpinBoxScaledMin->setEnabled(false);
        ui->doubleSpinBoxScaledMax->setEnabled(false);
    } else {
        ui->doubleSpinBoxScaledMin->setEnabled(true);
        ui->doubleSpinBoxScaledMax->setEnabled(true);
    }
}

void DecisionCreationWidget::on_radioButtonScaled_clicked(bool checked)
{
    // Enable scale doublespinbox widgets for scaled decision
    if (checked) {
        ui->doubleSpinBoxScaledMin->setEnabled(true);
        ui->doubleSpinBoxScaledMax->setEnabled(true);
    } else {
        ui->doubleSpinBoxScaledMin->setEnabled(false);
        ui->doubleSpinBoxScaledMax->setEnabled(false);
    }
}
