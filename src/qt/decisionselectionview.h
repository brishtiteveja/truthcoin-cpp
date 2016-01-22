#ifndef DECISIONSELECTIONVIEW_H
#define DECISIONSELECTIONVIEW_H

#include "decisionselectionmodel.h"

#include <QTableView>
#include <QWidget>

namespace Ui {
class DecisionSelectionView;
}

class DecisionSelectionView : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionSelectionView(QWidget *parent = 0);
    ~DecisionSelectionView();

public slots:
    void loadDecisions(QList<marketDecision *> decisions);
    void on_table_doubleClicked(QModelIndex index);

signals:
    void decisionSelected(QString decisionHex);
    void done();

private slots:
    void on_pushButtonDone_clicked();

private:
    Ui::DecisionSelectionView *ui;

    DecisionSelectionModel *decisionSelectionModel;
    QTableView *decisionSelectionTable;
};

#endif // DECISIONSELECTIONVIEW_H
