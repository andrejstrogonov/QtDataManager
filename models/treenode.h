#ifndef TREENODE_H
#define TREENODE_H

#include <QVariant>
#include <QVector>

class TreeNode
{
public:
    enum class Type {
        Root,
        Object,
        Transmitter,
        Spec
    };

    TreeNode(const QString &display, Type type = Type::Root, int id = -1, TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode *child);
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
    QVector<TreeNode*> m_children;
    QString m_display;
    Type m_type;
    int m_id;
    TreeNode *m_parent;
};

#endif // TREENODE_H
