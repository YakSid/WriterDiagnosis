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
    void onLwItemTextChanged(qint32 id, QString newText);

private:
    void _menuClosed();
    //! Заполнить словарь и комбинации из загруженного файла
    void _prepareFromExample(const SExample *example);
    //! Расставить слова по листВиджетам
    void _setWordsInLws();
    //! Найти минимальное свободное значение id
    qint32 _makeMinId();
    //! Обновить строчку, сколько сохранено вариаций диагноза
    void _updateDiadnosisCountLabel();

private:
    Ui::MainWindow *ui;

    QList<CCustomListWidget *> m_listWidgets;
    CSavingAccepter *m_savingAccepter { nullptr };
    CMenu *m_menu { nullptr };
    //! Весь текущий тест - полный список слов, их возможных позиций к сохранению и сохранённых комбинаций
    SExample *m_example { nullptr };
    QMap<qint32, QString> *m_dictionary { nullptr };
    QStringList *m_combinations { nullptr };
};

#endif // MAINWINDOW_H
