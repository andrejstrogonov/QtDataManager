#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QVector>

class TreeNode {
public:
    // Added Transmitter and Spec to match TreeModel usage
    enum class Type { Root, Object, Transmitter, Spec };

    explicit TreeNode(const QString &display, Type type, int id, TreeNode *parent = nullptr);
    // Added for compatibility with your previous Unit Tests
    explicit TreeNode(const QVector<QVariant> &data, TreeNode *parent = nullptr);
    
    ~TreeNode();

    void appendChild(TreeNode *child);
    void insertChild(int row, TreeNode *child);
    
    TreeNode *child(int row) const;
    int childCount() const;
    int columnCount() const { return 1; }
    
    QVariant data(int column) const;
    void setData(int column, const QVariant &value);
    
    int row() const;
    TreeNode *parent() const;
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
