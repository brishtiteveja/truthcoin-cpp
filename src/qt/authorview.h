#ifndef AUTHORVIEW_H
#define AUTHORVIEW_H

#include <QWidget>

namespace Ui {
class AuthorView;
}

class AuthorView : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorView(QWidget *parent = 0);
    ~AuthorView();

private:
    Ui::AuthorView *ui;
};

#endif // AUTHORVIEW_H
