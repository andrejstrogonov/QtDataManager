#include "treenode.h"

TreeNode::TreeNode(const QString &display, Type type, int id, TreeNode *parent)
    : m_display(display), m_type(type), m_id(id), m_parent(parent)
{
}

TreeNode::~TreeNode()
{
    qDeleteAll(m_children);
}

void TreeNode::appendChild(TreeNode *child)
{
    m_children.append(child);
}

void TreeNode::removeChild(int row)
{
    if (row < 0 || row >= m_children.size())
        return;
    delete m_children.takeAt(row);
}

TreeNode *TreeNode::child(int row) const
{
    if (row < 0 || row >= m_children.size())
        return nullptr;
    return m_children.at(row);
}

int TreeNode::childCount() const
{
    return m_children.count();
}

int TreeNode::columnCount() const
{
    return 1;
}

QVariant TreeNode::data(int column) const
{
    if (column != 0)
        return QVariant();
    return m_display;
}

void TreeNode::setData(int column, const QVariant &value)
{
    if (column != 0)
        return;
    m_display = value.toString();
}

int TreeNode::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<TreeNode*>(this));
    return 0;
}

TreeNode *TreeNode::parent()
{
    return m_parent;
}

TreeNode::Type TreeNode::type() const
{
    return m_type;
}

int TreeNode::id() const
{
    return m_id;
}
