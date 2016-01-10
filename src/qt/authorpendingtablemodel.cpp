#include "authorpendingtablemodel.h"
#include "primitives/market.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QFont>

AuthorPendingTableModel::AuthorPendingTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int AuthorPendingTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return pending.size();
}

int AuthorPendingTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

QVariant AuthorPendingTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch(role) {
    case Qt::DisplayRole:
    {
        json_spirit::Array pendingCreation = pending.at(row);

        // Type
        if (col == 0) {
            std::string type = json_spirit::write_string(pendingCreation.back(), true);
            return QString::fromStdString(type);
        }

        // Address
        if (col == 1) {
            std::string address = json_spirit::write_string(pendingCreation.front(), true);
            return QString::fromStdString(address);
        }

        // Fee
        if (col == 2) {
            return "0.0"; // Dummy temp
        }

        break;
    }
    case Qt::FontRole:
        if (col == 0) // Type is bold
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:
        break;
    case Qt::TextAlignmentRole:
        break;
    case Qt::CheckStateRole:
        break;
    }

    return QVariant();
}

QVariant AuthorPendingTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Type");
            case 1:
                return QString("Address");
            case 2:
                return QString("Fee");
            }
        }
    }
    return QVariant();
}

void AuthorPendingTableModel::editCombo(const QModelIndex &index)
{
    comboCreationWidget = new ComboCreationWidget;

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(comboCreationWidget);

    connect(comboCreationWidget, SIGNAL(updatedComboArray(json_spirit::Array)),
            this, SLOT(receiveUpdatedCombo(json_spirit::Array)));

    QDialog *dialog = new QDialog;
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorPendingTableModel::editDecision(const QModelIndex &index)
{
    DecisionCreationWidget *decisionCreationWidget = new DecisionCreationWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(decisionCreationWidget);

    connect(decisionCreationWidget, SIGNAL(updatedDecisionArray(json_spirit::Array)),
            this, SLOT(receiveUpdatedDecision(json_spirit::Array)));

    json_spirit::Array decisionToEdit = pending.at(index.row());
    decisionToEdit.push_back(index.row());
    decisionCreationWidget->editArray(decisionToEdit);

    QDialog *dialog = new QDialog;
    dialog->setLayout(hbox);
    dialog->setWindowTitle("Edit Decision");
    dialog->show();
}

void AuthorPendingTableModel::editDecisionMarket(const QModelIndex &index)
{
    DecisionMarketCreationWidget *marketCreationWidget = new DecisionMarketCreationWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(marketCreationWidget);

    connect(marketCreationWidget, SIGNAL(updatedDecisionMarketArray(json_spirit::Array)),
            this, SLOT(receiveUpdatedDecisionMarket(json_spirit::Array)));

    json_spirit::Array marketToEdit = pending.at(index.row());
    marketToEdit.push_back(index.row());
    marketCreationWidget->editArray(marketToEdit);

    QDialog *dialog = new QDialog;
    dialog->setLayout(hbox);
    dialog->setWindowTitle("Edit Market");
    dialog->show();
}

void AuthorPendingTableModel::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Figure out what we are editing, and open proper dialog
    json_spirit::Array itemClicked = pending.at(index.row());
    std::string itemType = json_spirit::write_string(itemClicked.back(), true);

    if (itemType == "\"combo\"") {
        editCombo(index);
    } else if (itemType == "\"decision\"") {
        editDecision(index);
    } else if (itemType == "\"market\"") {
        editDecisionMarket(index);
    } else {
        return;
    }
}

void AuthorPendingTableModel::receivePendingCombo(json_spirit::Array array)
{
    pending.push_back(array);
}

void AuthorPendingTableModel::receivePendingDecision(json_spirit::Array array)
{
    int row = pending.size();
    beginInsertRows(QModelIndex(), row, row);

    pending.push_back(array);

    endInsertRows();
}

void AuthorPendingTableModel::receivePendingDecisionMarket(json_spirit::Array array)
{
    int row = pending.size();
    beginInsertRows(QModelIndex(), row, row);

    pending.push_back(array);

    endInsertRows();
}

void AuthorPendingTableModel::finalize()
{
    if (pending.empty()) {
        QString error = "There is nothing to finalize, create some decisions or markets!\n";
        emit finalizeError(error);
    }

    // Finalize the pending creations
    for (int i = 0; i < pending.size(); i++) {
        // Grab creation parameters and type
        json_spirit::Array params = pending.at(i);
        std::string type = json_spirit::write_string(params.back(), true);

        // Remove type from parameters
        params.pop_back();

        extern json_spirit::Value createdecision(const json_spirit::Array &params, bool fHelp);
        extern json_spirit::Value createmarket(const json_spirit::Array &params, bool fHelp);

        json_spirit::Value result;

        // Try to finalize pending creations
        try {
            if (type == "\"combo\"") {

            } else if (type == "\"decision\"") {
                result = createdecision(params, false);
            } else if (type == "\"market\"") {
                result = createmarket(params, false);
            } else {
                continue;
            }
        } catch (const std::runtime_error &error) {
            QString errorText = QString::fromStdString(error.what());
            emit finalizeError(errorText);
        } catch (const std::exception &exception) {
            QString exceptionText = QString::fromStdString(exception.what());
            emit finalizeError(exceptionText);
        } catch (json_spirit::Object &object) {
            result = object;
        }

        // Check the result
        try {
            // Get result pairs
            json_spirit::Object resultObject = result.get_obj();
            json_spirit::Pair codePair = resultObject[0];
            json_spirit::Pair messagePair = resultObject[1];

            // If error, get error code and message
            if (codePair.name_ == "code") {
                int code = codePair.value_.get_int();
                if (code < 0) {
                    QString messageText = "Error creating #";
                    messageText.append(QString::number(i+1)); // row #
                    messageText.append("\n");
                    messageText.append(QString::fromStdString(messagePair.value_.get_str()));
                    emit finalizeError(messageText);
                }
            }

            // If success, get txid and decision or market id
            if (codePair.name_ == "txid" ) {
                std::string txid = codePair.value_.get_str();
                // Decision or market?
                if (messagePair.name_ == "decisionid") {
                    std::string decisionID = messagePair.value_.get_str();
                    // Remove completed creations from the pending model
                    beginRemoveRows(QModelIndex(), i, i);

                    pending.removeAt(i);

                    endRemoveRows();
                } else if (messagePair.name_ == "marketid") {
                    std::string marketID = messagePair.value_.get_str();
                    // Remove completed creations from the pending model
                    beginRemoveRows(QModelIndex(), i, i);

                    pending.removeAt(i);

                    endRemoveRows();
                }
            }
        } catch (const std::runtime_error &error) {
            QString errorText = QString::fromStdString(error.what());
            emit finalizeError(errorText);
        } catch (const std::exception &exception) {
            QString exceptionText = QString::fromStdString(exception.what());
            emit finalizeError(exceptionText);
        }
    }
}

void AuthorPendingTableModel::receiveUpdatedCombo(json_spirit::Array array)
{

}

void AuthorPendingTableModel::receiveUpdatedDecision(json_spirit::Array array)
{
    if (!array.size() >= 8) return;
    int index = array.back().get_int();
    array.pop_back();

    pending.replace(index, array);
}

void AuthorPendingTableModel::receiveUpdatedDecisionMarket(json_spirit::Array array)
{
    if(!array.size() == 13) return;
    int index = array.back().get_int();
    array.pop_back();

    pending.replace(index, array);
}
