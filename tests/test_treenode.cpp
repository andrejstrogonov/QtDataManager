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

QTEST_MAIN(TestTreeNode)
#include "test_treenode.moc"
