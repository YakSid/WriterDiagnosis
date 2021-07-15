#include "csavingaccepter.h"
#include "ui_csavingaccepter.h"
#include <QLabel>
#include <QCheckBox>

CSavingAccepter::CSavingAccepter(QWidget *parent) : QDialog(parent), ui(new Ui::CSavingAccepter)
{
    ui->setupUi(this);

    // TODO: переделать добавление лейаутов
    auto verticalLayout = new QVBoxLayout(ui->scrollAreaMain);
    for (int i = 0; i < 14; i++) {
        auto layout = new QHBoxLayout();
        auto lbl = new QLabel("пример\nтекста", this);
        auto ch = new QCheckBox(this);
        layout->addWidget(lbl);
        layout->addWidget(ch);
        verticalLayout->addItem(layout);
    }
}

CSavingAccepter::~CSavingAccepter()
{
    delete ui;
}
