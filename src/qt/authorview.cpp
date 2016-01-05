#include "authorview.h"
#include "ui_authorview.h"

#include <QHBoxLayout>
#include <QDialog>

AuthorView::AuthorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorView)
{
    ui->setupUi(this);

    // Setup model & author pending table
    pendingTableView = new QTableView(this);
    pendingTableModel = new AuthorPendingTableModel(this);
    pendingTableView->setModel(pendingTableModel);
    ui->frameLeft->layout()->addWidget(pendingTableView);

    // Setup signals
    connect(this, SIGNAL(newPendingCombo(json_spirit::Array)),
            pendingTableModel, SLOT(receivePendingCombo(json_spirit::Array)));

    connect(this, SIGNAL(newPendingDecision(json_spirit::Array)),
            pendingTableModel, SLOT(receivePendingDecision(json_spirit::Array)));

    connect(this, SIGNAL(newPendingDecisionMarket(json_spirit::Array)),
            pendingTableModel, SLOT(receivePendingDecisionMarket(json_spirit::Array)));

    connect(pendingTableView, SIGNAL(doubleClicked(QModelIndex)),
            pendingTableModel, SLOT(on_tableView_doubleClicked(QModelIndex)));
}

AuthorView::~AuthorView()
{
    delete ui;
}

void AuthorView::on_pushButtonCreateCombo_clicked()
{
    comboCreationWidget = new ComboCreationWidget(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(comboCreationWidget);

    // Pass array to model
    connect(comboCreationWidget, SIGNAL(receivedComboArray(json_spirit::Array)),
            this, SIGNAL(newPendingCombo(json_spirit::Array)));
    connect(comboCreationWidget, SIGNAL(receivedComboArray(json_spirit::Array)),
            this, SIGNAL(newPendingCombo(json_spirit::Array)));

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Create Combo (Decision & Market)");
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorView::on_pushButtonCreateDecision_clicked()
{
    decisionCreationWidget = new DecisionCreationWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(decisionCreationWidget);

    // Pass array to model
    connect(decisionCreationWidget, SIGNAL(receivedDecisionArray(json_spirit::Array)),
            this, SIGNAL(newPendingDecision(json_spirit::Array)));

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Create Decision");
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorView::on_pushButtonCreateMarket_clicked()
{
    decisionMarketCreationWidget = new DecisionMarketCreationWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(decisionMarketCreationWidget);

    // Pass array to model
    connect(decisionMarketCreationWidget, SIGNAL(receivedDecisionMarketArray(json_spirit::Array)),
            this, SIGNAL(newPendingDecisionMarket(json_spirit::Array)));

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Create Market");
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorView::on_pushButtonFinalize_clicked()
{
    // CREATE DECISION CODE
    // Remove the type value from the array
    //params.pop_back();

//    // Create decision, passing spirit array and returning spirit object
//    json_spirit::Value result;
//    try {
//        result = createdecision(params, false);
//    } catch (const std::runtime_error &error) {
//        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
//        std::cout << "Error: \n" << error.what() << std::endl;
//        return;
//    } catch (const std::exception &exception) {
//        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
//        std::cout << "Exception: \n" << exception.what() << std::endl;
//        return;
//    } catch (const json_spirit::Object &object) {
//        result = object;
//    } catch (...) {
//        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
//        std::cout << "Unknown Exception!\n";
//        return;
//    }

//    // Unpack spirit results
//    try {
//        std::string text = json_spirit::write_string(result, true);
//        std::cout << "Create Decision Result: \n" << text << std::endl;
//    } catch (...) {
//        std::cout << "decisioncreationwidget::on_pushButtonCreateDecision clicked\n";
//        std::cout << "write_string: Unknown Exception!\n";
//    }


    // CREATE MARKET CODE
//    // Create market, passing spirit array and returning spirit object
//    json_spirit::Value result;
//    try {
//        result = createmarket(params, false);
//    } catch (const std::runtime_error &error) {
//        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "Error: \n" << error.what() << std::endl;
//        return;
//    } catch (const std::exception &exception) {
//        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "Exception: \n" << exception.what() << std::endl;
//        return;
//    }  catch (const json_spirit::Object &object) {
//        result = object;
//    } catch (...) {
//        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "Unknown Exception!\n";
//        return;
//    }

//    // Unpack spirit results
//    try {
//        std::string text = json_spirit::write_string(result, true);
//        std::cout << "Create Market Result: " << text << std::endl;
//    } catch (...) {
//        std::cout << "decisioncreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "write_string: Unknown Exception!\n";
//    }
}
