#ifndef TREENODE_H
#define TREENODE_H

#include <QVector>
#include <QVariant>

class TreeNode
{
public:
    // The constructor used in your tests
    explicit TreeNode(const QVector<QVariant> &data, TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode *child);
    void insertChild(int row, TreeNode *child);
    void removeChild(TreeNode *child);

    TreeNode *child(int row);
    int childCount() const;
    int row() const;
    TreeNode *parent();
    QVariant data(int column) const;

private:
    QVector<TreeNode*> m_children;
    QVector<QVariant> m_itemData;
    TreeNode *m_parentItem;
};

#endif // TREENODE_H

