#include "decisionmarketcreationwidget.h"
#include "ui_decisionmarketcreationwidget.h"

#include "json/json_spirit_writer_template.h"
#include "txdb.h"

#include <QMessageBox>
#include <QDialog>
#include <QHBoxLayout>

extern CMarketTreeDB *pmarkettree;

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
    std::string decisionID = ui->lineEditDecisions->text().toStdString();
    decisionID += ":";
    decisionID += ui->comboBoxFunctions->currentText().toStdString();
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

    if (decisionID.size() == 0) {
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
    params.push_back(decisionID);
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
    // Grab the ID of the user selected branch
    QString branchID;
    if (ui->comboBoxBranch->currentText() == "Main") {
        branchID = "0f894a25c5e0318ee148fe54600ebbf50782f0a1df1eb2aab06321a8ccec270d";
    } else if (ui->comboBoxBranch->currentText() == "Sports") {
        branchID = "419cd87761f45c108a976ca6d93d4929c7c4d1ff4386f5089fc2f7ff7ae21ddf";
    } else if (ui->comboBoxBranch->currentText() == "Econ") {
        branchID = "3277b5057ac9cda54e9edfbb45fd8bab38be1b5afc3cd6c587f6d17779f34f74";
    }

    // Exit if no branch is selected (technically impossible via the UI)
    if (branchID.isEmpty()) return;

    // Grab the branch
    uint256 uBranch;
    uBranch.SetHex(branchID.toStdString());
    const marketBranch *branch = pmarkettree->GetBranch(uBranch);

    if (!branch) return;

    // Grab decisions on the branch
    vector<marketDecision *> decisions = pmarkettree->GetDecisions(uBranch);

    // Setup the decision selection widget
    decisionSelection = new DecisionSelectionView(this);
    QVector<marketDecision *> qvDecisions = QVector<marketDecision *>::fromStdVector(decisions);
    QList<marketDecision *> qlDecisions = QList<marketDecision *>::fromVector(qvDecisions);
    decisionSelection->loadDecisions(qlDecisions);

    // Connect signals
    connect(decisionSelection, SIGNAL(decisionSelected(QString)),
            this, SLOT(decisionSelected(QString)));

    // Display the decision selection widget
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(decisionSelection);
    QDialog *dialog = new QDialog(this);

    connect(decisionSelection, SIGNAL(done()),
            dialog, SLOT(close()));

    dialog->setWindowTitle("Select Decision");
    dialog->setLayout(hbox);
    dialog->exec();
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
    ui->doubleSpinBoxDifficulty->setValue(txPoWd.get_real());

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

void DecisionMarketCreationWidget::comboCreationUI()
{
    ui->groupBoxDecision->hide();
    ui->pushButtonCreateMarket->hide();

    ui->lineEditAuthorAddress->setText("Temp");
    ui->lineEditDecisions->setText("Temp");

    ui->lineEditAuthorAddress->hide();
    ui->labelAuthorAddress->hide();
}

void DecisionMarketCreationWidget::decisionSelected(QString decisionHex)
{
    ui->lineEditDecisions->setText(decisionHex);
}
