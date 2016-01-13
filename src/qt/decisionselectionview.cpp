#include "decisionselectionview.h"
#include "ui_decisionselectionview.h"

DecisionSelectionView::DecisionSelectionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecisionSelectionView)
{
    ui->setupUi(this);
}

DecisionSelectionView::~DecisionSelectionView()
{
    delete ui;
}
