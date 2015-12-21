#ifndef HIVEMINDAUTHORPAGE_H
#define HIVEMINDAUTHORPAGE_H

#include <QWidget>

namespace Ui {
class HivemindAuthorPage;
}

class HivemindAuthorPage : public QWidget
{
    Q_OBJECT

public:
    explicit HivemindAuthorPage(QWidget *parent = 0);
    ~HivemindAuthorPage();

private:
    Ui::HivemindAuthorPage *ui;
};

#endif // HIVEMINDAUTHORPAGE_H
