#ifndef CSAVINGACCEPTER_H
#define CSAVINGACCEPTER_H

#include <QDialog>

//Окно открывается для показа всех возможных получившихся вариантов текстов

namespace Ui {
class CSavingAccepter;
}

class CSavingAccepter : public QDialog
{
    Q_OBJECT

public:
    explicit CSavingAccepter(QWidget *parent = nullptr);
    ~CSavingAccepter();

private:
    Ui::CSavingAccepter *ui;
};

#endif // CSAVINGACCEPTER_H
