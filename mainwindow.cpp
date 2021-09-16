#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "cjsonmanager.h"

const auto PROPERTY_BLOCK_TYPE = QByteArrayLiteral("blockType");

// TODO: на будущее: сделать хоткей для добавления фразы, например "+" по координатам курсора
// TODO: на будущее: сделать иконку программы и иконку для действия удаления в контекстном меню

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_dictionary = new QMap<qint32, QString>;
    m_combinations = new QStringList;
    m_savingAccepter = new CSavingAccepter(m_combinations, m_dictionary, this);
    connect(m_savingAccepter, &CSavingAccepter::s_lastDiagnosisDeleted, this, &MainWindow::slotLastDiagnosisDeleted);
    connect(m_savingAccepter, &CSavingAccepter::s_startSave, this, &MainWindow::startSave);

    //Кнопки
    QList<QPushButton *> pushButtons;
    pushButtons.append({ ui->pb_addMain, ui->pb_addBack, ui->pb_addComp, ui->pb_addAcc });
    qint8 blockType = 0;
    for (auto pb : pushButtons) {
        pb->setProperty(PROPERTY_BLOCK_TYPE, blockType);
        connect(pb, &QPushButton::clicked, this, &MainWindow::addWord);
        blockType++;
    }

    //Таблички и групбоксы
    //! Номер кнопки
    qint8 iPb = 0;
    QList<QGroupBox *> groupBoxes({ ui->gb_main, ui->gb_back, ui->gb_comp, ui->gb_acc });
    for (auto gb : groupBoxes) {
        auto lw = new CCustomListWidget(this);
        auto layout = new QHBoxLayout(gb);
        layout->addWidget(lw);
        layout->addWidget(pushButtons.at(iPb));
        iPb++;
        m_listWidgets.append(lw);
    }

    //ЛистВиджеты
    for (auto lw : m_listWidgets) {
        connect(lw, &CCustomListWidget::s_textChanged, this, &MainWindow::onLwItemTextChanged);
    }
    // TODO: при закрытии проверить m_dictionary на наличие слов, которые нигде не используются и удалить их?

    m_menu = new CMenu();
    m_menu->setModal(true);
    m_menu->exec();
    _menuClosed();
}

MainWindow::~MainWindow()
{
    if (m_example) {
        qDeleteAll(m_example->words);
        m_example->words.clear();
        delete m_example;
    }

    m_combinations->clear();
    delete m_combinations;
    delete m_savingAccepter;
    delete ui;
}

void MainWindow::addWord()
{
    auto pb = static_cast<QPushButton *>(sender());
    EBlockType block = EBlockType::main;
    if (pb)
        block = EBlockType(pb->property(PROPERTY_BLOCK_TYPE).toInt());

    auto lw = m_listWidgets.at(qint32(block));
    auto id = _makeMinId();
    auto item = new CLWItem(id);
    lw->addItem(item);
    lw->editItem(item);
    m_dictionary->insert(id, item->text());
}

void MainWindow::slotLastDiagnosisDeleted()
{
    ui->pb_further->setEnabled(false);
}

void MainWindow::startSave()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Сохранить проект"), QString(), tr("JSON (*.json)"));
    if (saveFileName.isEmpty()) {
        QMessageBox msg;
        msg.setText("Не выбран корректный путь сохранения");
        msg.exec();
        return;
    }

    m_example = new SExample();
    m_example->combinations = *m_combinations;

    //Берём каждое слово из словаря и создаём SWord
    for (auto it = m_dictionary->begin(); it != m_dictionary->end(); ++it) {
        //Записываем в него все возможные позиции слова проходясь по всем вариациям диагноза
        auto word = new SWord();
        word->id = it.key();
        word->text = it.value();
        for (auto comb : *m_combinations) {
            //Ищем сколько разделителей блоко перед нашим ключом слова - определяем disBlock
            auto pos = comb.indexOf(QString::number(it.key()));
            qint8 currentDisBlock = static_cast<qint8>(comb.left(pos).count(BLOCK_SEPARATOR));
            //Ищем позицию в блоке
            qint8 currentPos = -1;
            QStringList separeteds = comb.split(BLOCK_SEPARATOR);
            for (auto str : separeteds) {
                if (str.contains(it.key())) {
                    auto localPos = str.indexOf(QString::number(it.key()));
                    currentPos = static_cast<qint8>(str.left(localPos).count(SEPARATOR));
                    break;
                }
            }
            if (!word->availableDisBlock.contains(currentDisBlock))
                word->availableDisBlock.append(currentDisBlock);
            if (!word->availablePositions.contains(currentPos) && currentPos != -1)
                word->availableDisBlock.append(currentPos);
        }

        m_example->words.append(word);
    }

    CJsonManager::saveToFile(saveFileName, m_example);

    ui->pb_further->setEnabled(false);
    m_combinations->clear();
    if (m_example) {
        qDeleteAll(m_example->words);
        m_example->words.clear();
        delete m_example;
    }
    m_savingAccepter->clearVariables();
    m_example = nullptr;
    for (auto lw : m_listWidgets) {
        lw->clear();
    }
    _updateDiadnosisCountLabel();
    m_savingAccepter->close();
    m_menu->exec();
    _menuClosed();
}

void MainWindow::on_pb_further_clicked()
{
    m_savingAccepter->prepare();
    m_savingAccepter->setModal(true);
    m_savingAccepter->exec();

    _updateDiadnosisCountLabel();
}

void MainWindow::on_pb_saveDiagnosis_clicked()
{
    // WARNING: пример удалить
    QList<QStringList> allWords = {
        { "Ишемическая болезнь сердца", "Крупноочаговый кардиосклероз нижней стенки левого желудочка",
          "Стенозирующий атеросклероз коронарных артерий (3 степень 4 стадия стеноз до 50%)" },
        { "Гипертоническая болезнь: гипертрофия миокарда преимущественно левого желудочка (масса сердца – 570 г, "
          "толщина миокарда левого желудочка – 1,9 см, правого – 0,3 см, межжелудочковой перегородки – 1,9 см)",
          "Сахарный диабет 2 типа", "Диабетическая ангиопатия",
          "Диабетическая нефропатия (см. также клинические данные)", "Ожирение" },
        { "Застойная сердечная недостаточность", "«Мускатная» печень", "Цианотическая индурация селезенки и почек",
          "Двусторонний гидроторакс (1400 мл)", "Гидроперикард (200 мл)", "Асцит (8000 мл)",
          "Отёки подкожной клетчатки туловища и конечностей",
          "Дистрофические изменения внутренних органов и острое венозное полнокровие", "Отёк лёгких" },
        { "Атеросклероз аорты (3 степень, 4 стадия)", "Хронический панкреатит, ремиссия" }
    };

    QString diagnosis;
    for (auto lw : m_listWidgets) {
        for (auto i = 0; i < lw->count(); i++) {
            auto clwItem = static_cast<CLWItem *>(lw->item(i));
            diagnosis.append(QString::number(clwItem->getId()) + SEPARATOR);
        }
        diagnosis.append(BLOCK_SEPARATOR);
    }
    m_combinations->append(diagnosis);

    _updateDiadnosisCountLabel();

    if (!ui->pb_further->isEnabled())
        ui->pb_further->setEnabled(true);
}

void MainWindow::closeApp()
{
    qApp->quit();
}

void MainWindow::onLwItemTextChanged(qint32 id, QString newText)
{
    auto it = m_dictionary->find(id);
    it.value() = newText;
}

void MainWindow::_menuClosed()
{
    switch (m_menu->getMode()) {
    case 1:
        // Создание нового - ничего дополнительно делать не надо
        break;
    case 2: {
        // Загрузка старого
        QString openFileName = QFileDialog::getOpenFileName(this, tr("Открыть проект"), QString(), tr("JSON (*.json)"));
        if (openFileName.isEmpty()) {
            QMessageBox msg;
            msg.setText("Не выбран корректный путь к файлу, открыт новый проект");
            msg.exec();
        } else {
            auto example = CJsonManager::loadFromFile(openFileName);
            m_example = new SExample(example);
            _prepareFromExample(&example);
            _setWordsInLws();
            if (!m_combinations->isEmpty()) {
                ui->pb_further->setEnabled(true);
                _updateDiadnosisCountLabel();
            }
        }
    } break;
    default:
        // TODO: плохой костыль, нужно переделать
        QTimer::singleShot(50, this, &MainWindow::closeApp);
    }
    m_menu->updateMode();
}

void MainWindow::_prepareFromExample(const SExample *example)
{
    *m_combinations = example->combinations;

    for (auto word : example->words) {
        m_dictionary->insert(word->id, word->text);
    }
}

void MainWindow::_setWordsInLws()
{
    auto comb = m_combinations->first();
    auto separeteds = comb.split(BLOCK_SEPARATOR);
    for (int i = 0; i < separeteds.count(); i++) {
        auto keysInBlock = separeteds.at(i).split(SEPARATOR);
        for (auto key : keysInBlock) {
            auto keyInt = key.toInt();
            if (m_dictionary->find(keyInt) == m_dictionary->end())
                break;
            auto text = m_dictionary->find(keyInt).value();
            auto item = new CLWItem(keyInt);
            item->setText(text);
            m_listWidgets.at(i)->addItem(item);
        }
    }
}

qint32 MainWindow::_makeMinId()
{
    qint32 minId = 0;

    for (int i = 1; true; i++) {
        bool exist = false;
        if (m_dictionary->find(i) != m_dictionary->end()) {
            exist = true;
        }

        if (!exist) {
            minId = i;
            break;
        }
    }
    return minId;
}

void MainWindow::_updateDiadnosisCountLabel()
{
    ui->lb_diagnosisCount->setText("Сохранено вариаций диагноза: " + QString::number(m_combinations->count()));
}

// NOTE: указать, что изменение слова после сохранения диагноза изменит его также вов сех сохранённых диагнозах
// NOTE: вроде можно добавлять и удалять слова после сохранения проекта или диагноза
