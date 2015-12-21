#include "hivemindauthorpage.h"
#include "ui_hivemindauthorpage.h"

HivemindAuthorPage::HivemindAuthorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HivemindAuthorPage)
{
    ui->setupUi(this);
}

HivemindAuthorPage::~HivemindAuthorPage()
{
    delete ui;
}
