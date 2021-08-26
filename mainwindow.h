#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "ccustomlistwidget.h"
#include "csavingaccepter.h"
#include "cstructs.h"
#include "cmenu.h"

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
    void slotLastDiagnosisDeleted();
    //! Начать сохранение вариаций диагноза в файл
    void startSave();

private slots:
    void on_pb_further_clicked();
    void on_pb_saveDiagnosis_clicked();
    void closeApp();

private:
    void _menuClosed();
    //! Заполнить m_savedDiagnosis из загруженного файла
    void _prepareSavedDiagnosisFromLoadedFile(const SExample *example);
    //! Расставить слова по листВиджетам
    void _setWordsInLws(const SExample *example);

private:
    Ui::MainWindow *ui;

    QList<CCustomListWidget *> m_listWidgets;
    CSavingAccepter *m_savingAccepter { nullptr };
    CMenu *m_menu { nullptr };
    //! Список сохранённых диагнозов
    QList<QList<QStringList>> *m_savedDiagnosis { nullptr };
    //! Весь текущий тест - полный список слов и их возможных позиций к сохранению
    SExample *m_example { nullptr };
};

#endif // MAINWINDOW_H
