#include "cmenu.h"
#include "ui_cmenu.h"

CMenu::CMenu(QWidget *parent) : QDialog(parent), ui(new Ui::CMenu)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
}

CMenu::~CMenu()
{
    delete ui;
}

void CMenu::on_pb_create_clicked()
{
    // TODO: на будущее: сделать enum
    m_mode = 1;
    close();
}

void CMenu::on_pb_edit_clicked()
{
    m_mode = 2;
    close();
}
