#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

const auto PROPERTY_BLOCK_TYPE = QByteArrayLiteral("blockType");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_listWidgets.append({ ui->lw_mainDis, ui->lw_backgroundDis, ui->lw_complications, ui->lw_accompanying });
    QList<QPushButton *> pushButtons;
    pushButtons.append({ ui->pb_addMain, ui->pb_addBack, ui->pb_addComp, ui->pb_addAcc });
    qint8 blockType = 0;
    for (auto pb : pushButtons) {
        pb->setProperty(PROPERTY_BLOCK_TYPE, blockType);
        connect(pb, &QPushButton::clicked, this, &MainWindow::addWord);
        blockType++;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addWord()
{
    auto pb = static_cast<QPushButton *>(sender());
    EBlockType block = EBlockType::main;
    if (pb)
        block = EBlockType(pb->property(PROPERTY_BLOCK_TYPE).toInt());

    m_listWidgets.at(qint32(block))->addItem("Слово");
}
