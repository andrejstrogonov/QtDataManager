#include "spectablemodel.h"
#include "dbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SpecTableModel::SpecTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void SpecTableModel::setTransmitterId(int transmitterId)
{
    if (m_transmitterId == transmitterId)
        return;
    m_transmitterId = transmitterId;
    load();
    emit layoutChanged();
}

int SpecTableModel::transmitterId() const
{
    return m_transmitterId;
}

int SpecTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rows.count();
}

int SpecTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2; // parameter + value
}

QVariant SpecTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.count())
        return QVariant();

    const RowData &row = m_rows.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return row.label;
        if (index.column() == 1)
            return row.value;
    }
    return QVariant();
}

QVariant SpecTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return tr("Параметр");
        if (section == 1)
            return tr("Значение");
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags SpecTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    if (index.column() == 1)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool SpecTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || index.column() != 1)
        return false;

    if (index.row() < 0 || index.row() >= m_rows.count())
        return false;

    RowData &row = m_rows[index.row()];
    QVariant newValue = value;

    QSqlDatabase db = DbManager::instance().database();
    if (!db.isOpen())
        return false;

    QSqlQuery query(db);
    QString sql = QStringLiteral("UPDATE specs SET %1 = :val WHERE id = :id").arg(row.field);
    query.prepare(sql);
    query.bindValue(":val", newValue);
    query.bindValue(":id", m_specId);
    if (!query.exec()) {
        qDebug() << "Failed to update spec:" << query.lastError().text();
        return false;
    }

    row.value = newValue;
    emit dataChanged(index, index, {role});
    return true;
}

void SpecTableModel::ensureSpecRow()
{
    if (m_transmitterId < 0)
        return;

    QSqlDatabase db = DbManager::instance().database();
    if (!db.isOpen())
        return;

    QSqlQuery query(db);
    query.prepare("SELECT id, power_watt, gain_db, antenna_height FROM specs WHERE transmitter_id = :tid");
    query.bindValue(":tid", m_transmitterId);
    if (!query.exec()) {
        qDebug() << "Failed to query specs:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        m_specId = query.value(0).toInt();
        // use last values
        return;
    }

    // create default spec row if missing
    query.prepare("INSERT INTO specs (transmitter_id, power_watt, gain_db, antenna_height) VALUES (:tid, 0, 0, 0)");
    query.bindValue(":tid", m_transmitterId);
    if (!query.exec()) {
        qDebug() << "Failed to insert default spec row:" << query.lastError().text();
        return;
    }

    m_specId = query.lastInsertId().toInt();
}

void SpecTableModel::load()
{
    m_rows.clear();
    m_specId = -1;

    if (m_transmitterId < 0)
        return;

    ensureSpecRow();
    if (m_specId < 0)
        return;

    QSqlDatabase db = DbManager::instance().database();
    if (!db.isOpen())
        return;

    QSqlQuery query(db);
    query.prepare("SELECT power_watt, gain_db, antenna_height FROM specs WHERE id = :id");
    query.bindValue(":id", m_specId);
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to load spec values:" << query.lastError().text();
        return;
    }

    m_rows.append({tr("Мощность (Вт)"), QStringLiteral("power_watt"), query.value(0)});
    m_rows.append({tr("КУ (дБ)"), QStringLiteral("gain_db"), query.value(1)});
    m_rows.append({tr("Высота подвеса (м)"), QStringLiteral("antenna_height"), query.value(2)});
}
