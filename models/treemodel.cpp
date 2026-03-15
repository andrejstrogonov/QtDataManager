#include "treemodel.h"
#include "treenode.h"
#include "dbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeNode("Root", TreeNode::Type::Root, -1);
    loadFromDatabase();
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

void TreeModel::reload()
{
    beginResetModel();
    delete m_rootItem;
    m_rootItem = new TreeNode("Root", TreeNode::Type::Root, -1);
    loadFromDatabase();
    endResetModel();
}

int TreeModel::transmitterId(const QModelIndex &index) const
{
    if (!index.isValid())
        return -1;
    TreeNode *node = static_cast<TreeNode*>(index.internalPointer());
    if (node->type() == TreeNode::Type::Transmitter)
        return node->id();
    // Если выбран объект — попробуем найти транслирующую строку
    return -1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    return item->data(index.column());
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    if (!item)
        return false;

    // Only allow renaming objects and transmitters
    if (item->type() == TreeNode::Type::Object) {
        QSqlQuery query(DbManager::instance().database());
        query.prepare("UPDATE objects SET name = :name WHERE id = :id");
        query.bindValue(":name", value.toString());
        query.bindValue(":id", item->id());
        if (!query.exec()) {
            qDebug() << "Failed to update object name:" << query.lastError().text();
            return false;
        }
    } else if (item->type() == TreeNode::Type::Transmitter) {
        QSqlQuery query(DbManager::instance().database());
        query.prepare("UPDATE transmitters SET name = :name WHERE id = :id");
        query.bindValue(":name", value.toString());
        query.bindValue(":id", item->id());
        if (!query.exec()) {
            qDebug() << "Failed to update transmitter name:" << query.lastError().text();
            return false;
        }
    } else {
        return false;
    }

    // Update in-memory node and inform views
    item->setData(0, value);
    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->data(section);
    }
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    TreeNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

// in treemodel.cpp
QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();

    TreeNode *childItem = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentItem = childItem->parent();

    // The root item has no parent index in QAbstractItemModel
    if (parentItem == m_rootItem || !parentItem) return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeNode*>(parent.internalPointer())->columnCount();
    return m_rootItem->columnCount();
}

void TreeModel::loadFromDatabase()
{
    // Load objects -> transmitters -> specs hierarchy
    QSqlDatabase db = DbManager::instance().database();
    if (!db.isOpen())
        return;

    QSqlQuery query(db);
    if (!query.exec("SELECT id, name FROM objects ORDER BY name")) {
        qDebug() << query.lastError().text();
        return;
    }

    while (query.next()) {
        int objId = query.value(0).toInt();
        QString objName = query.value(1).toString();
        TreeNode *objNode = new TreeNode(objName, TreeNode::Type::Object, objId, m_rootItem);
        m_rootItem->appendChild(objNode);

        QSqlQuery txQuery(db);
        txQuery.prepare("SELECT id, name FROM transmitters WHERE object_id = :oid ORDER BY name");
        txQuery.bindValue(":oid", objId);
        if (txQuery.exec()) {
            while (txQuery.next()) {
                int txId = txQuery.value(0).toInt();
                QString txName = txQuery.value(1).toString();
                TreeNode *txNode = new TreeNode(txName, TreeNode::Type::Transmitter, txId, objNode);
                objNode->appendChild(txNode);

                QSqlQuery specQuery(db);
                specQuery.prepare("SELECT parameter_name, parameter_value FROM view_transmitter_specs WHERE transmitter_id = :tid ORDER BY parameter_name");
                specQuery.bindValue(":tid", txId);
                if (specQuery.exec()) {
                    while (specQuery.next()) {
                        QString param = specQuery.value(0).toString();
                        QString value = specQuery.value(1).toString();
                        TreeNode *specNode = new TreeNode(param + ": " + value, TreeNode::Type::Spec, txId, txNode);
                        txNode->appendChild(specNode);
                    }
                }
            }
        }
    }
}

bool TreeModel::addItem(const QString &parentType, const QVariantList &values)
{
    Q_UNUSED(parentType);
    Q_UNUSED(values);
    return false;
}

bool TreeModel::removeItem(const QModelIndex &index)
{
    if (!index.isValid())
        return false;

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    if (!item)
        return false;

    // only allow deleting objects or transmitters from the database
    if (item->type() == TreeNode::Type::Spec)
        return false;

    QSqlDatabase db = DbManager::instance().database();
    if (!db.isOpen())
        return false;

    QSqlQuery query(db);
    if (item->type() == TreeNode::Type::Object) {
        query.prepare("DELETE FROM objects WHERE id = :id");
        query.bindValue(":id", item->id());
        if (!query.exec()) {
            qDebug() << "Failed to delete object:" << query.lastError().text();
            return false;
        }
    } else if (item->type() == TreeNode::Type::Transmitter) {
        query.prepare("DELETE FROM transmitters WHERE id = :id");
        query.bindValue(":id", item->id());
        if (!query.exec()) {
            qDebug() << "Failed to delete transmitter:" << query.lastError().text();
            return false;
        }
    }

    // reload model after database change
    reload();
    return true;
}
