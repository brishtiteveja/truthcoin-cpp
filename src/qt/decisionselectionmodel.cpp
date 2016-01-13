#include "decisionselectionmodel.h"

DecisionSelectionModel::DecisionSelectionModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int DecisionSelectionModel::rowCount(const QModelIndex &parent) const
{
    return decisions.size();
}

int DecisionSelectionModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant DecisionSelectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return false;
    }

    int row = index.row();
    int col = index.column();

    switch(role) {
    case Qt::DisplayRole:
    {
        // Decision ID
        if (col == 0) {
            return QString("DecisionID");
        }
        break;
    }
    }

    return QVariant();

}

QVariant DecisionSelectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("DecisionID");
            }
        }
    }
    return QVariant();
}
