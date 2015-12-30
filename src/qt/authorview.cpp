#include "authorview.h"
#include "ui_authorview.h"

#include <QHBoxLayout>
#include <QDialog>

AuthorView::AuthorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorView)
{
    ui->setupUi(this);
}

AuthorView::~AuthorView()
{
    delete ui;
}

void AuthorView::on_pushButtonCreateCombo_clicked()
{
    ComboCreationWidget *comboCreationWidget = new ComboCreationWidget(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(comboCreationWidget);

    QDialog *dialog = new QDialog(this);
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorView::on_pushButtonCreateDecision_clicked()
{
    DecisionCreationWidget *creationWidget = new DecisionCreationWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(creationWidget);

    // Recieve decision arrays from each creation window the user opens
    connect(creationWidget, SIGNAL(receivedDecisionArray(json_spirit::Array)),
            this, SLOT(on_decisionArray_received(json_spirit::Array)));

    QDialog *dialog = new QDialog(this);
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorView::on_pushButtonCreateMarket_clicked()
{
    DecisionMarketCreationWidget *marketCreationWidget = new DecisionMarketCreationWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(marketCreationWidget);

    QDialog *dialog = new QDialog(this);
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorView::on_decisionArray_received(const json_spirit::Array &array)
{
    std::cout << "Decision array received\n";
    std::cout << "----------------------------------------\n";

    for (unsigned int i = 0; i < array.size(); i++) {
        std::string text = json_spirit::write_string(array.at(i), true);
        std::cout << text << std::endl;
    }

    std::cout << "----------------------------------------\n";
}
