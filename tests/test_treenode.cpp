#ifndef TEST_TREENODE_H
#define TEST_TREENODE_H

#include <QtTest/QtTest>
#include "models/treenode.h"

class TestTreeNode : public QObject
{
    Q_OBJECT

private slots:
    void testAppendChild();
    void testRowAndParent();
};

void TestTreeNode::testAppendChild()
{
    QVector<QVariant> data({"root"});
    TreeNode root(data);
    QCOMPARE(root.childCount(), 0);

    TreeNode *child = new TreeNode({"child"}, &root);
    root.appendChild(child);
    QCOMPARE(root.childCount(), 1);
    QCOMPARE(root.child(0)->data(0).toString(), QString("child"));
}

void TestTreeNode::testRowAndParent()
{
    TreeNode root({"r"});
    TreeNode *c1 = new TreeNode({"c1"}, &root);
    TreeNode *c2 = new TreeNode({"c2"}, &root);
    root.appendChild(c1);
    root.appendChild(c2);

    QCOMPARE(c1->row(), 0);
    QCOMPARE(c2->row(), 1);
    QCOMPARE(c1->parent()->data(0).toString(), QString("r"));
}

// Additional tests
void TestTreeNode::testRemoveChild()
{
    TreeNode root({"root"});
    TreeNode *child1 = new TreeNode({"child1"}, &root);
    TreeNode *child2 = new TreeNode({"child2"}, &root);
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
    QCOMPARE(node.data(3), QVariant()); // Out of bounds
}

void TestTreeNode::testInsertChild()
{
    TreeNode root({"root"});
    TreeNode *child1 = new TreeNode({"child1"}, &root);
    TreeNode *child2 = new TreeNode({"child2"}, &root);
    root.appendChild(child1);
    
    // Insert at beginning
    root.insertChild(0, child2);
    QCOMPARE(root.child(0)->data(0).toString(), QString("child2"));
    QCOMPARE(root.child(1)->data(0).toString(), QString("child1"));
}


QTEST_MAIN(TestTreeNode)
#include "test_treenode.moc"
