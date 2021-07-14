#ifndef CCUSTOMLISTWIDGET_H
#define CCUSTOMLISTWIDGET_H

#include <QObject>
#include <QMouseEvent>
#include <clwitem.h>

class CCustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CCustomListWidget(QWidget *parent = nullptr);

signals:

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);

private:
    void _rightButtonRelease(QPoint pos);
};

#endif // CCUSTOMLISTWIDGET_H
