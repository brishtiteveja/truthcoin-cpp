#include "combocreationwidget.h"
#include "ui_combocreationwidget.h"

#include "decisioncreationwidget.h"

ComboCreationWidget::ComboCreationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboCreationWidget)
{
    ui->setupUi(this);

    decisionCreationWidget = new DecisionCreationWidget(this);
    decisionCreationWidget->comboCreationUI();

    marketCreationWidget = new DecisionMarketCreationWidget(this);
    marketCreationWidget->comboCreationUI();

    ui->frameDecision->layout()->addWidget(decisionCreationWidget);
    ui->frameMarket->layout()->addWidget(marketCreationWidget);
}

ComboCreationWidget::~ComboCreationWidget()
{
    delete ui;
}

void ComboCreationWidget::on_pushButtonCreateCombo_clicked()
{

}
