#ifndef CSAVINGACCEPTER_H
#define CSAVINGACCEPTER_H

#include <QDialog>
#include <QMap>

//Окно открывается для показа всех возможных получившихся вариантов текстов

// TODO: на потом сделать сохранение в ini размеров окна, которые были заданы перед закрытием

namespace Ui {
class CSavingAccepter;
}

class CSavingAccepter : public QDialog
{
    Q_OBJECT

public:
    explicit CSavingAccepter(QStringList *combinations, QMap<qint32, QString> *dictionary, QWidget *parent = nullptr);
    ~CSavingAccepter();

    //! Подготовить экран заполнив его диагнозами
    void prepare();
    //! Обнулить переменные для начала работы с другим проектом
    void clearVariables();

private slots:
    void on_pb_prevDiagnosis_clicked();
    void on_pb_nextDiagnosis_clicked();
    void on_pb_deleteDiagnosis_clicked();
    void on_pb_back_clicked();
    void on_pb_save_clicked();

signals:
    //! Удалены все вариации диагноза
    void s_lastDiagnosisDeleted();
    //! Все существующие вариации диагноза одобрены, нужно начать сохранение в файл
    void s_startSave();

private:
    //Комбинаторика
    //! Проверяет есть ли ещё комбинация и если есть, то преобразует в неё
    bool _nextCombination(QList<qint32> &list);
    //! Возвращает полученную комбинацию
    QList<qint32> _saveCombination(const QList<qint32> &list);
    //! Сформировать строчку словаря в соответствии с расстановкой ключей
    QString _useFakeCombinationToSortWords(const QList<qint32> &list, const QMap<qint32, QString> &wordsCopy);

    //! Найти в списке диагноз и отобразить его по форме
    void _showDiagnosis(qint32 id);

private:
    Ui::CSavingAccepter *ui;

    //! Указатель на словарь из главного окна
    QMap<qint32, QString> *m_dictionary { nullptr };
    //! Указатель на список комбинаций из главного окна
    QStringList *m_combinations { nullptr };
    //! Номер текущей показываемой комбинации диагноза
    qint32 m_currentDiagnosisId = 0;
};

#endif // CSAVINGACCEPTER_H
