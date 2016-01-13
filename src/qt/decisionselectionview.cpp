#include "decisionselectionview.h"
#include "ui_decisionselectionview.h"

DecisionSelectionView::DecisionSelectionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionSelectionView)
{
    ui->setupUi(this);

    // Setup model and decision selection table
    decisionSelectionTable = new QTableView(this);
    decisionSelectionModel = new DecisionSelectionModel(this);
    decisionSelectionTable->setModel(decisionSelectionModel);

    ui->frame->layout()->addWidget(decisionSelectionTable);
}

DecisionSelectionView::~DecisionSelectionView()
{
    delete ui;
}
