#ifndef CLWITEM_H
#define CLWITEM_H

#include <QListWidgetItem>

///     0. Хранить данные
///     3. По ПКМ удаляться

class CLWItem : public QListWidgetItem
{
public:
    explicit CLWItem(QListWidget *parent = nullptr);
    explicit CLWItem(qint32 id, QListWidget *parent = nullptr);
    ~CLWItem();
    qint32 getId() { return m_id; }

private:
    qint32 m_id { -1 };
};

#endif // CLWITEM_H
