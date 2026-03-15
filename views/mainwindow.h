#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class TreeModel;
class QItemSelection;
class SpecTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeView_selectionChanged(const QItemSelection &selected,
                                      const QItemSelection &deselected);
    void addItem();
    void removeItem();

private:
    Ui::MainWindow *ui;
    TreeModel *m_model;
    SpecTableModel *m_specModel = nullptr;
};

#endif // MAINWINDOW_H
