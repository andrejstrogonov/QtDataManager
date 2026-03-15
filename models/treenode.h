#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QVector>

class TreeNode {
public:
    enum class Type { Root, Object, Transmitter, Spec };

    // Конструкторы
    explicit TreeNode(const QString &display, Type type, int id, TreeNode *parent = nullptr);
    explicit TreeNode(const QVector<QVariant> &data, TreeNode *parent = nullptr);
    ~TreeNode();

    // Методы для работы с дочерними элементами
    void appendChild(TreeNode *child);
    void insertChild(int row, TreeNode *child); // Добавлено
    void removeChild(TreeNode *child);         // Добавлено

    TreeNode *child(int row) const;
    int childCount() const;
    int columnCount() const;
    
    // Работа с данными
    QVariant data(int column) const;
    void setData(int column, const QVariant &value);
    
    // Иерархия
    int row() const;
    TreeNode *parent() const;
    Type type() const;
    int id() const;

private:
    QVector<QVariant> m_itemData; // Изменено для поддержки нескольких колонок
    Type m_type;
    int m_id;
    TreeNode *m_parent;
    QList<TreeNode*> m_children;
};

#endif


