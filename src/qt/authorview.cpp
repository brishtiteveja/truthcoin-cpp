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
