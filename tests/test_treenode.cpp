#include <QtTest/QtTest>
#include <QVector>
#include <QVariant>
#include "models/treenode.h"

class TestTreeNode : public QObject
{
    Q_OBJECT

private slots:
    void testAppendChild();
    void testRowAndParent();
    void testRemoveChild();
    void testData();
    void testInsertChild();
    void testConstruction();
    void testChildCount();
};

void TestTreeNode::testAppendChild()
{
    QVector<QVariant> data({"root"});
    TreeNode root(data);
    QCOMPARE(root.childCount(), 0);

    TreeNode *child = new TreeNode(QVector<QVariant>({"child"}), &root);
    root.appendChild(child);
    QCOMPARE(root.childCount(), 1);
    QCOMPARE(root.child(0)->data(0).toString(), QString("child"));
}

void TestTreeNode::testRowAndParent()
{
    TreeNode root(QVector<QVariant>({"r"}));
    TreeNode *c1 = new TreeNode(QVector<QVariant>({"c1"}), &root);
    TreeNode *c2 = new TreeNode(QVector<QVariant>({"c2"}), &root);
    root.appendChild(c1);
    root.appendChild(c2);

    QCOMPARE(c1->row(), 0);
    QCOMPARE(c2->row(), 1);
    QCOMPARE(c1->parent()->data(0).toString(), QString("r"));
}

void TestTreeNode::testRemoveChild()
{
    TreeNode root(QVector<QVariant>({"root"}));
    TreeNode *child1 = new TreeNode(QVector<QVariant>({"child1"}), &root);
    TreeNode *child2 = new TreeNode(QVector<QVariant>({"child2"}), &root);
    root.appendChild(child1);
    root.appendChild(child2);
    
    QCOMPARE(root.childCount(), 2);
    root.removeChild(child1);
    QCOMPARE(root.childCount(), 1);
    QCOMPARE(root.child(0)->data(0).toString(), QString("child2"));
}

void TestTreeNode::testData()
{
    QVector<QVariant> data({"test", "data", "values"});
    TreeNode node(data);
    
    QCOMPARE(node.data(0).toString(), QString("test"));
    QCOMPARE(node.data(1).toString(), QString("data"));
    QCOMPARE(node.data(2).toString(), QString("values"));
    QCOMPARE(node.data(3), QVariant()); 
}

void TestTreeNode::testInsertChild()
{
    TreeNode root(QVector<QVariant>({"root"}));
    TreeNode *child1 = new TreeNode(QVector<QVariant>({"child1"}), &root);
    TreeNode *child2 = new TreeNode(QVector<QVariant>({"child2"}), &root);
    root.appendChild(child1);
    
    root.insertChild(0, child2);
    QCOMPARE(root.child(0)->data(0).toString(), QString("child2"));
    QCOMPARE(root.child(1)->data(0).toString(), QString("child1"));
}

void TestTreeNode::testConstruction()
{
    QVector<QVariant> data({"node"});
    TreeNode node(data);
    QCOMPARE(node.data(0).toString(), QString("node"));
    QCOMPARE(node.parent(), nullptr);
}

void TestTreeNode::testChildCount()
{
    TreeNode root(QVector<QVariant>({"root"}));
    QCOMPARE(root.childCount(), 0);
    root.appendChild(new TreeNode(QVector<QVariant>({"c1"})));
    QCOMPARE(root.childCount(), 1);
}

QTEST_MAIN(TestTreeNode)
// This include tells MOC to generate code for the Q_OBJECT in this .cpp file
#include "test_treenode.moc"
