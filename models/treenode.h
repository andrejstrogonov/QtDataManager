#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QVector>

class TreeNode {
public:
    // Added Object, Transmitter, Spec, and Root to match your Model logic
    enum class Type { Root, Object, Transmitter, Spec };

    explicit TreeNode(const QString &display, Type type, int id, TreeNode *parent = nullptr);
    explicit TreeNode(const QVector<QVariant> &data, TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode *child);
    
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

#endif // TREENODE_H

