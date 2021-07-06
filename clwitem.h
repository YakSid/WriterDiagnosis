#ifndef CLWITEM_H
#define CLWITEM_H

#include <QObject>

///Это должен быть наследуемый от QListWidgetItem класс поддерживающий:
///     0. Хранить данные
///     1. Отправлять сигнал, что нажат и отправлять данные
///     2. Открывать диалоговое меню по нажатию ПКМ
///     3. По ПКМ удаляться
///     4. По ПКМ редактироваться
///     5. По ПКМ перемещаться

class CLWItem : public QObject
{
    Q_OBJECT
public:
    explicit CLWItem(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CLWITEM_H
