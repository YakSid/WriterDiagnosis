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
    void s_textChanged(qint32 itemId, QString newText);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);

private:
    void _rightButtonRelease(QPoint pos);
    void onItemChanged(QListWidgetItem *item);
};

#endif // CCUSTOMLISTWIDGET_H
