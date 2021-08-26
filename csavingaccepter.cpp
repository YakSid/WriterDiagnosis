#include "csavingaccepter.h"
#include "ui_csavingaccepter.h"
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
#include "cstructs.h"

CSavingAccepter::CSavingAccepter(QList<QList<QStringList>> *savedDiagnosis, QWidget *parent)
    : QDialog(parent), ui(new Ui::CSavingAccepter)
{
    m_savedDiagnosis = savedDiagnosis;
    ui->setupUi(this);
    ui->gb_all->hide();
}

CSavingAccepter::~CSavingAccepter()
{
    delete ui;
}

void CSavingAccepter::prepare()
{
    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);

    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_savedDiagnosis->count()));
}

void CSavingAccepter::clearVariables()
{
    m_currentDiagnosisId = 0;
}

/*void CSavingAccepter::fillWords(const QList<QStringList> &allWords)
{
    // К старой версии: Учесть если 0 в lw было. Переделать с энумом счётчик lw.
      // Сделать грамотные условия выборки и если вариаций меньше ~30, то показывать
      // Выводить можно как текст например.

    QList<QVBoxLayout *> vLayouts = { ui->vLayoutMain, ui->vLayoutBack, ui->vLayoutComp, ui->vLayoutAcc };
    qint8 layNumber = 0;

    for (auto list : allWords) {
        auto currentVLayout = vLayouts.at(layNumber);
        // Формируем список всех комбинаций
        QStringList allCombinations;
        //Подменяем список мапкой, чтобы делать лексиграфический перебор по ключам
        QMap<qint32, QString> map;
        QList<qint32> fakeList; //фейковый лист, чтобы формировать строку теми словами, как были сформированы ключи
        for (int i = 0; i < list.count(); i++) {
            map.insert(i, list.at(i));
            fakeList.append(i);
        }

        //добавить смену курсора на ожидание

        allCombinations.append(_useFakeCombinationToSortWords(_saveCombination(fakeList), map));
        while (_nextCombination(fakeList)) {
            allCombinations.append(_useFakeCombinationToSortWords(_saveCombination(fakeList), map));
        }
        qDebug() << "Combinations:" << allCombinations.count();

        // Выводим все комбинации в ui
        if (allCombinations.count() < 120) {
            for (int i = 0; i < allCombinations.count(); i++) {
                auto hLayout = new QHBoxLayout;
                hLayout->setContentsMargins(0, 0, 0, 0);
                hLayout->setSpacing(0);
                auto lbl = new QLabel(allCombinations.at(i), this);
                auto ch = new QCheckBox(this);
                hLayout->addWidget(lbl);
                hLayout->addWidget(ch);
                currentVLayout->addLayout(hLayout);
            }
        } else {
            qDebug() << "Слишком много комбинаций, уменьшите количество";
        }
        layNumber++;
    }
}*/

bool CSavingAccepter::_nextCombination(QList<qint32> &list)
{
    qint32 j = list.count() - 2;

    while (j != -1 && list[j] >= list[j + 1])
        j--;

    if (j == -1)
        return false; // больше перестановок нет

    qint32 k = list.count() - 1;

    while (list[j] >= list[k])
        k--;

    list.swap(j, k);

    // Сортируем оставшуюся часть последовательности
    qint32 l = j + 1;
    qint32 r = list.count() - 1;

    while (l < r)
        list.swap(l++, r--);

    return true;
}

QList<qint32> CSavingAccepter::_saveCombination(const QList<qint32> &list)
{
    QList<qint32> result = list;
    return result;
}

QString CSavingAccepter::_useFakeCombinationToSortWords(const QList<qint32> &list,
                                                        const QMap<qint32, QString> &wordsCopy)
{
    QString result;
    for (auto key : list) {
        result += wordsCopy.value(key) + " ";
    }
    result.chop(1);
    return result;
}

void CSavingAccepter::_showDiagnosis(qint32 id)
{
    if (!m_savedDiagnosis) {
        qDebug() << "Ошибка: список диагнозов не инициализирован";
        return;
    }

    if (m_savedDiagnosis->count() < id || id < 0) {
        qDebug() << "Ошибка: невалидный номер диагноза";
        return;
    }

    QString result;
    for (int block = 0; block < m_savedDiagnosis->at(id).count(); block++) {
        switch (block) {
        case 0:
            result += "Основное заболевание:\n";
            break;
        case 1:
            result += "Фоновое заболевание:\n";
            break;
        case 2:
            result += "Осложнения основного заболевания:\n";
            break;
        case 3:
            result += "Сопутствующие заболевания:\n";
            break;
        default:
            break;
        }

        for (auto word : m_savedDiagnosis->at(id).at(block)) {
            result += word + ". ";
        }
        result.chop(1);
        result += "\n\n";
    }
    result.chop(2);
    ui->te_diagnosis->setText(result);

    //Форматирование блоков
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setFontItalic(true);

    for (int i = 0; i < 4; i++) {
        QString header;
        if (i == 0) {
            header = "Основное заболевание:";
        } else if (i == 1) {
            header = "Фоновое заболевание:";
        } else if (i == 2) {
            header = "Осложнения основного заболевания:";
        } else if (i == 3) {
            header = "Сопутствующие заболевания:";
        }
        auto posStart = result.indexOf(header);
        auto posEnd = posStart + header.count();
        auto textCursor = ui->te_diagnosis->textCursor();
        textCursor.setPosition(posStart, QTextCursor::MoveAnchor);
        textCursor.setPosition(posEnd, QTextCursor::KeepAnchor);
        textCursor.setCharFormat(format);
    }
}

void CSavingAccepter::on_pb_prevDiagnosis_clicked()
{
    if (m_currentDiagnosisId == 0) {
        //Первый диагноз из списка
        m_currentDiagnosisId = m_savedDiagnosis->count() - 1;
    } else {
        m_currentDiagnosisId--;
    }

    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);

    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_savedDiagnosis->count()));
}

void CSavingAccepter::on_pb_nextDiagnosis_clicked()
{
    if (m_currentDiagnosisId == m_savedDiagnosis->count() - 1) {
        //Последний диагноз из списка
        m_currentDiagnosisId = 0;
    } else {
        m_currentDiagnosisId++;
    }

    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);

    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_savedDiagnosis->count()));
}

void CSavingAccepter::on_pb_deleteDiagnosis_clicked()
{
    m_savedDiagnosis->removeAt(m_currentDiagnosisId);

    if (m_savedDiagnosis->count() == 0) {
        emit s_lastDiagnosisDeleted();
        close();
        return;
    } else if (m_currentDiagnosisId >= m_savedDiagnosis->count() - 1) {
        m_currentDiagnosisId--;
    }

    if (m_currentDiagnosisId < 0)
        m_currentDiagnosisId = 0;

    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);
    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_savedDiagnosis->count()));
}

void CSavingAccepter::on_pb_back_clicked()
{
    close();
}

void CSavingAccepter::on_pb_save_clicked()
{
    emit s_startSave();
    close();
}
