#include <QtTest/QtTest>
#include <QStandardItemModel>
#include <QLineEdit>
#include "controllers/valuedelegate.h"

class TestValueDelegate : public QObject
{
    Q_OBJECT

private slots:
    void testEditCycle();
};

void TestValueDelegate::testEditCycle()
{
    QStandardItemModel model(1,1);
    model.setItem(0,0, new QStandardItem("initial"));

    ValueDelegate delegate;
    QModelIndex idx = model.index(0,0);

    QWidget *editor = delegate.createEditor(nullptr, QStyleOptionViewItem(), idx);
    delegate.setEditorData(editor, idx);
    QLineEdit *line = qobject_cast<QLineEdit*>(editor);
    QVERIFY(line);
    QCOMPARE(line->text(), QString("initial"));

    line->setText("changed");
    delegate.setModelData(editor, &model, idx);
    QCOMPARE(model.data(idx).toString(), QString("changed"));
    delete editor;
}

QTEST_MAIN(TestValueDelegate)
#include "test_valuedelegate.moc"
