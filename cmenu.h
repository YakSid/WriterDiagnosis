#ifndef CMENU_H
#define CMENU_H

#include <QDialog>

namespace Ui {
class CMenu;
}

class CMenu : public QDialog
{
    Q_OBJECT

public:
    explicit CMenu(QWidget *parent = nullptr);
    ~CMenu();
    quint8 getMode() { return m_mode; }
    void updateMode() { m_mode = 0; }

private slots:
    void on_pb_create_clicked();
    void on_pb_edit_clicked();

private:
    Ui::CMenu *ui;

    quint8 m_mode = 0;
};

#endif // CMENU_H
