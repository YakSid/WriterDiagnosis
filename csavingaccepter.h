#ifndef CSAVINGACCEPTER_H
#define CSAVINGACCEPTER_H

#include <QDialog>
#include <QMap>

//Окно открывается для показа всех возможных получившихся вариантов текстов

// TODO: на потом сделать сохранение в ini размеров окна, которые были заданы перед закрытием
// TODO: убрать знак вопроса сверху справа, как в kef в kda если можно

namespace Ui {
class CSavingAccepter;
}

class CSavingAccepter : public QDialog
{
    Q_OBJECT

public:
    explicit CSavingAccepter(QList<QList<QStringList>> *savedDiagnosis, QWidget *parent = nullptr);
    ~CSavingAccepter();

    //! Подготовить экран заполнив его диагнозами
    void prepare();

private slots:
    void on_pb_prevDiagnosis_clicked();
    void on_pb_nextDiagnosis_clicked();
    void on_pb_deleteDiagnosis_clicked();
    void on_pb_back_clicked();
    void on_pb_save_clicked();

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

    //! Указатель на список сохранённых диагнозов из главного окна
    QList<QList<QStringList>> *m_savedDiagnosis;
    qint32 m_currentDiagnosisId = 0;
};

#endif // CSAVINGACCEPTER_H
