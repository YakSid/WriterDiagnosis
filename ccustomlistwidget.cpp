#include "ccustomlistwidget.h"
#include <QDebug>
#include <QMenu>

CCustomListWidget::CCustomListWidget(QWidget *parent) : QListWidget(parent)
{
    setFont(QFont("Times New Roman", 11));
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void CCustomListWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton) {
        _rightButtonRelease(ev->pos());
    } else {
        QListWidget::mouseReleaseEvent(ev);
    }
}

void CCustomListWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    auto item = itemAt(ev->pos());
    if (item)
        editItem(item);
}

void CCustomListWidget::_rightButtonRelease(QPoint pos)
{
    auto item = itemAt(pos);
    if (!item)
        return;

    QMenu menu;
    auto actionDelete = new QAction("Удалить", this);
    menu.addAction(actionDelete);

    auto selectedAction = menu.exec(mapToGlobal(pos));
    if (selectedAction == actionDelete) {
        auto item = takeItem(currentRow());
        delete item;
    }
    delete actionDelete;
}
