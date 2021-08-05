#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "ccustomlistwidget.h"
#include "csavingaccepter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum class EBlockType { main, back, comp, acc };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addWord();

private slots:
    void on_pb_further_clicked();

    void on_pb_saveDiagnosis_clicked();

private:
    Ui::MainWindow *ui;

    QList<CCustomListWidget *> m_listWidgets;
    CSavingAccepter *m_savingAccepter;
    //! Список сохранённых диагнозов
    QList<QList<QStringList>> *m_savedDiagnosis { nullptr };
};

#endif // MAINWINDOW_H
