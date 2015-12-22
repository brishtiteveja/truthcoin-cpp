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
