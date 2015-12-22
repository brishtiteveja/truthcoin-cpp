#include "authorview.h"
#include "ui_authorview.h"

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

}

void AuthorView::on_pushButtonCreateDecision_clicked()
{

}

void AuthorView::on_pushButtonCreateMarket_clicked()
{

}
