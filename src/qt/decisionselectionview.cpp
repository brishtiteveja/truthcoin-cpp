#include "decisionselectionview.h"
#include "ui_decisionselectionview.h"

DecisionSelectionView::DecisionSelectionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionSelectionView)
{
    ui->setupUi(this);

    // Setup model and decision selection table
    decisionSelectionTable = new QTableView(this);
    decisionSelectionTable->horizontalHeader()->setStretchLastSection(true);
    decisionSelectionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    decisionSelectionModel = new DecisionSelectionModel(this);
    decisionSelectionTable->setModel(decisionSelectionModel);

    ui->frame->layout()->addWidget(decisionSelectionTable);

    // Setup signals
    connect(decisionSelectionTable, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(on_table_doubleClicked(QModelIndex)));
}

DecisionSelectionView::~DecisionSelectionView()
{
    delete ui;
}

void DecisionSelectionView::loadDecisions(QList<marketDecision *> decisions)
{
    decisionSelectionModel->loadDecisions(decisions);
}

void DecisionSelectionView::on_table_doubleClicked(QModelIndex index)
{
    QString hex = decisionSelectionTable->model()->data(decisionSelectionTable->model()->index(index.row(), 1)).toString();
    emit decisionSelected(hex);
    emit done();
}

void DecisionSelectionView::on_pushButtonDone_clicked()
{
    emit done();
}
