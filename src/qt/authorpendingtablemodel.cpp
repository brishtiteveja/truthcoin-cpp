#include "authorpendingtablemodel.h"
#include "primitives/market.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QFont>
#include <QBrush>

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

    QDialog *dialog = new QDialog;
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorPendingTableModel::editDecision(const QModelIndex &index)
{
    DecisionCreationWidget *creationWidget = new DecisionCreationWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(creationWidget);

    QDialog *dialog = new QDialog;
    dialog->setLayout(hbox);
    dialog->show();
}

void AuthorPendingTableModel::editDecisionMarket(const QModelIndex &index)
{
    DecisionMarketCreationWidget *marketCreationWidget = new DecisionMarketCreationWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(marketCreationWidget);

    QDialog *dialog = new QDialog;
    dialog->setLayout(hbox);
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

        json_spirit::Value result;

        extern json_spirit::Value createdecision(const json_spirit::Array &params, bool fHelp);

        // Try to finalize pending creations
        if (type == "\"combo\"") {

        } else if (type == "\"decision\"") {
            try {
                result = createdecision(params, false);
            } catch (const std::runtime_error &error) {
                QString errorText = QString::fromStdString(error.what());
                emit finalizeError(errorText);
            } catch (const std::exception &exception) {
                QString exceptionText = QString::fromStdString(exception.what());
                emit finalizeError(exceptionText);
            } catch (json_spirit::Object &object) {
                result = object;
            }
        } else if (type == "\"market\"") {

        } else {
            return;
        }

        // Check the result
        try {
            // Get result pairs
            json_spirit::Object resultObject = result.get_obj();
            json_spirit::Pair codePair = resultObject[0];
            json_spirit::Pair messagePair = resultObject[1];

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
        } catch (const std::runtime_error &error) {
            QString errorText = QString::fromStdString(error.what());
            emit finalizeError(errorText);
        } catch (const std::exception &exception) {
            QString exceptionText = QString::fromStdString(exception.what());
            emit finalizeError(exceptionText);
        }
    }

    // CREATE MARKET CODE
//    // Create market, passing spirit array and returning spirit object
//    json_spirit::Value result;
//    try {
//        result = createmarket(params, false);
//    } catch (const std::runtime_error &error) {
//        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "Error: \n" << error.what() << std::endl;
//        return;
//    } catch (const std::exception &exception) {
//        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "Exception: \n" << exception.what() << std::endl;
//        return;
//    }  catch (const json_spirit::Object &object) {
//        result = object;
//    } catch (...) {
//        std::cout << "decisionmarketcreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "Unknown Exception!\n";
//        return;
//    }

//    // Unpack spirit results
//    try {
//        std::string text = json_spirit::write_string(result, true);
//        std::cout << "Create Market Result: " << text << std::endl;
//    } catch (...) {
//        std::cout << "decisioncreationwidget::on_pushButtonCreateMarket clicked\n";
//        std::cout << "write_string: Unknown Exception!\n";
//    }
}
