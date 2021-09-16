#include "csavingaccepter.h"
#include "ui_csavingaccepter.h"
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
#include "cstructs.h"

CSavingAccepter::CSavingAccepter(QStringList *combinations, QMap<qint32, QString> *dictionary, QWidget *parent)
    : QDialog(parent), ui(new Ui::CSavingAccepter)
{
    m_dictionary = dictionary;
    m_combinations = combinations;
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
                                    + QString::number(m_combinations->count()));
}

void CSavingAccepter::clearVariables()
{
    m_currentDiagnosisId = 0;
}

void CSavingAccepter::_showDiagnosis(qint32 id)
{
    if (!m_combinations) {
        qDebug() << "Ошибка: список диагнозов не инициализирован";
        return;
    }

    if (m_combinations->count() < id || id < 0) {
        qDebug() << "Ошибка: невалидный номер диагноза";
        return;
    }

    QString result;
    auto comb = m_combinations->at(id);
    auto blocks = comb.split(BLOCK_SEPARATOR);
    qint8 blockCounter = 0;
    for (auto block : blocks) {
        //Указываем заголовок
        switch (blockCounter) {
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
        blockCounter++;
        //Записываем слова
        auto keysInBlock = block.split(SEPARATOR);
        for (auto key : keysInBlock) {
            auto keyInt = key.toInt();
            if (m_dictionary->find(keyInt) == m_dictionary->end())
                break;
            auto text = m_dictionary->find(keyInt).value();
            result += text + ". ";
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
        m_currentDiagnosisId = m_combinations->count() - 1;
    } else {
        m_currentDiagnosisId--;
    }

    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);

    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_combinations->count()));
}

void CSavingAccepter::on_pb_nextDiagnosis_clicked()
{
    if (m_currentDiagnosisId == m_combinations->count() - 1) {
        //Последний диагноз из списка
        m_currentDiagnosisId = 0;
    } else {
        m_currentDiagnosisId++;
    }

    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);

    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_combinations->count()));
}

void CSavingAccepter::on_pb_deleteDiagnosis_clicked()
{
    m_combinations->removeAt(m_currentDiagnosisId);

    if (m_combinations->count() == 0) {
        emit s_lastDiagnosisDeleted();
        close();
        return;
    } else if (m_currentDiagnosisId >= m_combinations->count() - 1) {
        m_currentDiagnosisId--;
    }

    if (m_currentDiagnosisId < 0)
        m_currentDiagnosisId = 0;

    //Заполнить диагноз из списка по форме
    _showDiagnosis(m_currentDiagnosisId);
    //Заполнить остальные элементы
    ui->lb_diagnosisNumber->setText(QString::number(m_currentDiagnosisId + 1) + " / "
                                    + QString::number(m_combinations->count()));
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
