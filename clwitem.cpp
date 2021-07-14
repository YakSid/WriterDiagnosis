#include "clwitem.h"
#include <QDebug>

CLWItem::CLWItem(QListWidget *parent) : QListWidgetItem(parent)
{
    setText("Введите текст");
    this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled
                   | Qt::ItemIsEditable);
}

CLWItem::CLWItem(QString content, QListWidget *parent) : QListWidgetItem(content, parent) {}

CLWItem::~CLWItem() {}
