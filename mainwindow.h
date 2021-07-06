#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

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

private:
    Ui::MainWindow *ui;

    QList<QListWidget *> m_listWidgets;
};

#endif // MAINWINDOW_H
