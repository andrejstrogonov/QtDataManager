#include "treenode.h"

// Основной конструктор
TreeNode::TreeNode(const QString &display, Type type, int id, TreeNode *parent)
    : m_display(display), 
      m_type(type), 
      m_id(id), 
      m_parent(parent) 
{
}

// Конструктор для тестов (принимает QVector)
TreeNode::TreeNode(const QVector<QVariant> &data, TreeNode *parent)
    : m_type(Root), 
      m_id(0), 
      m_parent(parent)
{
    if (!data.isEmpty()) {
        m_display = data.at(0).toString();
    }
}

TreeNode::~TreeNode() {
    qDeleteAll(m_children);
}

void TreeNode::appendChild(TreeNode *child) {
    if (child) {
        child->m_parent = this; // Важно: устанавливаем родителя!
        m_children.append(child);
    }
}

void TreeNode::insertChild(int row, TreeNode *child) {
    if (child && row >= 0 && row <= m_children.size()) {
        child->m_parent = this;
        m_children.insert(row, child);
    }
}

void TreeNode::removeChild(int row) {
    if (row >= 0 && row < m_children.size()) {
        delete m_children.takeAt(row);
    }
}

// Перегрузка для теста: удаление по указателю
void TreeNode::removeChild(TreeNode *child) {
    int index = m_children.indexOf(child);
    if (index != -1) {
        delete m_children.takeAt(index);
    }
}

TreeNode *TreeNode::child(int row) const {
    if (row < 0 || row >= m_children.size())
        return nullptr;
    return m_children.at(row);
}

int TreeNode::childCount() const {
    return m_children.count();
}

int TreeNode::columnCount() const {
    return 1;
}

QVariant TreeNode::data(int column) const {
    // В тестах проверяются колонки 0, 1, 2. 
    // Если у нас только m_display, возвращаем его для колонки 0.
    if (column == 0) return m_display;
    return QVariant();
}

void TreeNode::setData(int column, const QVariant &value) {
    if (column == 0) m_display = value.toString();
}

int TreeNode::row() const {
    if (m_parent) {
        return m_parent->m_children.indexOf(const_cast<TreeNode*>(this));
    }
    return 0;
}

TreeNode *TreeNode::parent() {
    return m_parent;
}

TreeNode::Type TreeNode::type() const {
    return m_type;
}

int TreeNode::id() const {
    return m_id;
}