#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QVector>

class TreeNode {
public:
    enum class Type { Root, Object, Transmitter, Spec };

    explicit TreeNode(const QString &display, Type type = Type::Root, int id = -1, TreeNode *parent = nullptr);
    explicit TreeNode(const QVector<QVariant> &data, TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode *child);
    void insertChild(int row, TreeNode *child);
    void removeChild(TreeNode *child);

    TreeNode *child(int row) const;
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, const QVariant &value);
    
    int row() const;
    TreeNode *parent() const;
    Type type() const;
    int id() const;

private:
    QVector<QVariant> m_itemData;
    Type m_type;
    int m_id;
    TreeNode *m_parent;
    QList<TreeNode*> m_children;
};

#endif


