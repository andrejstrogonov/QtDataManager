#include "treenode.h"

TreeNode::TreeNode(const QString &display, Type type, int id, TreeNode *parent)
    : m_display(display), m_type(type), m_id(id), m_parent(parent) {}

TreeNode::TreeNode(const QVector<QVariant> &data, TreeNode *parent)
    : m_type(Type::Root), m_id(-1), m_parent(parent) 
{
    if (!data.isEmpty()) m_display = data.at(0).toString();
}

TreeNode::~TreeNode() {
    qDeleteAll(m_children);
}

void TreeNode::appendChild(TreeNode *child) {
    if (child) {
        child->m_parent = this; // Critical for TreeModel index mapping
        m_children.append(child);
    }
}

void TreeNode::insertChild(int row, TreeNode *child) {
    if (child && row >= 0 && row <= m_children.size()) {
        child->m_parent = this;
        m_children.insert(row, child);
    }
}

TreeNode *TreeNode::child(int row) const {
    return (row >= 0 && row < m_children.size()) ? m_children.at(row) : nullptr;
}

int TreeNode::childCount() const {
    return m_children.count();
}

QVariant TreeNode::data(int column) const {
    return (column == 0) ? m_display : QVariant();
}

void TreeNode::setData(int column, const QVariant &value) {
    if (column == 0) m_display = value.toString();
}

int TreeNode::row() const {
    if (m_parent) return m_parent->m_children.indexOf(const_cast<TreeNode*>(this));
    return 0;
}

TreeNode *TreeNode::parent() const {
    return m_parent;
}

TreeNode::Type TreeNode::type() const {
    return m_type;
}

int TreeNode::id() const {
    return m_id;
}
