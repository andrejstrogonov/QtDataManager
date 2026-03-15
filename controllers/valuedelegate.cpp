#include "valuedelegate.h"
#include <QLineEdit>

ValueDelegate::ValueDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *ValueDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}

void ValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit)
        lineEdit->setText(value);
}

void ValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
        model->setData(index, lineEdit->text(), Qt::EditRole);
    }
}
