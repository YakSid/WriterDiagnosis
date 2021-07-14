#ifndef CLWITEM_H
#define CLWITEM_H

#include <QListWidgetItem>

///     0. Хранить данные
///     3. По ПКМ удаляться

class CLWItem : public QListWidgetItem
{
public:
    explicit CLWItem(QListWidget *parent = nullptr);
    explicit CLWItem(QString content, QListWidget *parent = nullptr);
    ~CLWItem();
};

#endif // CLWITEM_H
