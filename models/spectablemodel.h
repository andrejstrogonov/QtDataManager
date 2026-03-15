#ifndef SPECTABLEMODEL_H
#define SPECTABLEMODEL_H

#include <QAbstractTableModel>

class SpecTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SpecTableModel(QObject *parent = nullptr);

    void setTransmitterId(int transmitterId);
    int transmitterId() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    void load();
    void ensureSpecRow();

    struct RowData {
        QString label;
        QString field;
        QVariant value;
    };

    int m_transmitterId = -1;
    int m_specId = -1;
    QList<RowData> m_rows;
};

#endif // SPECTABLEMODEL_H
