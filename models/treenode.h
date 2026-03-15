#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QVector>
#include <QVariant>
#include <QList>

class TreeNode {
public:
    enum Type { Root, Category, Item }; // Добавьте типы, если их нет

    // Конструктор должен соответствовать вызову в .cpp
    explicit TreeNode(const QString &display = QString(), Type type = Root, int id = 0, TreeNode *parent = nullptr);
    
    // Для совместимости с вашим тестом (QVector<QVariant>)
    explicit TreeNode(const QVector<QVariant> &data, TreeNode *parent = nullptr);
    
    ~TreeNode();

    void appendChild(TreeNode *child);
    void insertChild(int row, TreeNode *child); // Добавлено для теста
    void removeChild(TreeNode *child);        // Добавлено для теста
    void removeChild(int row);

    TreeNode *child(int row) const;
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, const QVariant &value);
    int row() const;
    TreeNode *parent();
    
    Type type() const;
    int id() const;

private:
    QString m_display;
    Type m_type;
    int m_id;
    TreeNode *m_parent;
    QList<TreeNode*> m_children; 
};

#endif
