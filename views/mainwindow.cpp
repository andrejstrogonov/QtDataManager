#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models/treemodel.h"
#include "models/treenode.h"
#include "models/spectablemodel.h"
#include "controllers/valuedelegate.h"

#include <QItemSelectionModel>
#include <QInputDialog>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(new TreeModel(this)),
    m_specModel(nullptr)
{
    ui->setupUi(this);

    ui->treeView->setModel(m_model);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setItemDelegate(new ValueDelegate(this));

    connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::addItem);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::removeItem);

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::on_treeView_selectionChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_specModel;
}

void MainWindow::on_treeView_selectionChanged(const QItemSelection &selected,
                                              const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndex idx = selected.indexes().isEmpty() ? QModelIndex() : selected.indexes().first();

    if (!idx.isValid()) {
        ui->tableView->setModel(nullptr);
        return;
    }

    int txId = m_model->transmitterId(idx);
    if (txId < 0) {
        ui->tableView->setModel(nullptr);
        return;
    }

    if (!m_specModel) {
        m_specModel = new SpecTableModel(this);
        ui->tableView->setModel(m_specModel);
    }

    m_specModel->setTransmitterId(txId);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::addItem()
{
    QModelIndex idx = ui->treeView->currentIndex();

    // Add new object when nothing is selected
    if (!idx.isValid()) {
        bool ok = false;
        QString name = QInputDialog::getText(this, tr("Добавить объект"), tr("Имя объекта:"), QLineEdit::Normal, QString(), &ok);
        if (!ok || name.isEmpty())
            return;

        QSqlQuery query;
        query.prepare("INSERT INTO objects (name, latitude, longitude) VALUES (:name, 0, 0)");
        query.bindValue(":name", name);
        if (!query.exec()) {
            qDebug() << "Failed to insert object:" << query.lastError().text();
            return;
        }
        m_model->reload();
        return;
    }

    // Add new transmitter when an object is selected
    int txId = m_model->transmitterId(idx);
    if (txId < 0) {
        // Object selected
        TreeNode *item = static_cast<TreeNode*>(idx.internalPointer());
        if (item && item->type() == TreeNode::Type::Object) {
            bool ok = false;
            QString name = QInputDialog::getText(this, tr("Добавить передатчик"), tr("Имя передатчика:"), QLineEdit::Normal, QString(), &ok);
            if (!ok || name.isEmpty())
                return;

            QSqlQuery query;
            query.prepare("INSERT INTO transmitters (object_id, name) VALUES (:oid, :name)");
            query.bindValue(":oid", item->id());
            query.bindValue(":name", name);
            if (!query.exec()) {
                qDebug() << "Failed to insert transmitter:" << query.lastError().text();
                return;
            }
            // ensure specs row exists
            QVariant lastId = query.lastInsertId();
            if (lastId.isValid()) {
                QSqlQuery specQuery;
                specQuery.prepare("INSERT INTO specs (transmitter_id, power_watt, gain_db, antenna_height) VALUES (:tid, 0, 0, 0)");
                specQuery.bindValue(":tid", lastId);
                specQuery.exec();
            }
            m_model->reload();
        }
        return;
    }

    // If a transmitter is selected, focus on table editing (no new row concept)
    ui->tableView->setFocus();
}

void MainWindow::removeItem()
{
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid())
        return;

    TreeNode *item = static_cast<TreeNode*>(idx.internalPointer());
    if (!item)
        return;

    if (item->type() == TreeNode::Type::Spec) {
        // nothing to delete here
        return;
    }

    m_model->removeItem(idx);
    ui->tableView->setModel(nullptr);
}
