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

    m_savedDiagnosis = new QList<QList<QStringList>>;
    m_savingAccepter = new CSavingAccepter(m_savedDiagnosis, this);
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

    //
    /*QStringList words = {
        "Ишемическая болезнь сердца",
        "Крупноочаговый кардиосклероз нижней стенки левого желудочка",
        "Стенозирующий атеросклероз коронарных артерий (3 степень 4 стадия стеноз до 50%)",
        "Гипертоническая болезнь: гипертрофия миокарда преимущественно левого желудочка (масса сердца – 570 г, толщина "
        "миокарда левого желудочка – 1,9 см, правого – 0,3 см, межжелудочковой перегородки – 1,9 см)",
        "Сахарный диабет 2 типа",
        "Диабетическая ангиопатия",
        "Диабетическая нефропатия (см. также клинические данные)",
        "Ожирение",
        "Застойная сердечная недостаточность",
        "«Мускатная» печень",
        "Цианотическая индурация селезенки и почек",
        "Двусторонний гидроторакс (1400 мл)",
        "Гидроперикард (200 мл)",
        "Асцит (8000 мл)",
        "Отёки подкожной клетчатки туловища и конечностей",
        "Дистрофические изменения внутренних органов и острое венозное полнокровие",
        "Отёк лёгких",
        "Атеросклероз аорты (3 степень, 4 стадия)",
        "Хронический панкреатит, ремиссия",
    };
    for (int i = 0; i < words.count(); i++) {
        if (i < 3) {
            ui->lw_mainDis->addItem(words.at(i));
        } else if (i < 8) {
            ui->lw_backgroundDis->addItem(words.at(i));
        } else if (i < 17) {
            ui->lw_complications->addItem(words.at(i));
        } else {
            ui->lw_accompanying->addItem(words.at(i));
        }
    }*/
    //

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

    m_savedDiagnosis->clear();
    delete m_savedDiagnosis;
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
    auto item = new CLWItem();
    lw->addItem(item);
    lw->editItem(item);
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

    //Составляем словарь id : word, пройдясь по всем словам в первой вариации
    QMap<qint32, QString> vocabulary;
    qint32 id = 0;
    for (auto disBlock : m_savedDiagnosis->first()) {
        for (auto word : disBlock) {
            vocabulary.insert(id, word);
            id++;
        }
    }

    //Берём каждое слово из словаря и создаём SWord записывая в него все возможные позиции слова проходясь по всем
    //вариациям диагноза
    for (auto it = vocabulary.begin(); it != vocabulary.end(); ++it) {
        auto word = new SWord();
        word->text = it.value();
        //Поиск слова по вариациям и запись позиции в SWord
        for (auto diagnosis : *m_savedDiagnosis) {
            qint8 currentDisBlock = 0;
            for (auto disBlock : diagnosis) {
                auto index = static_cast<qint8>(disBlock.indexOf(word->text));
                if (index != -1) {
                    if (!word->availableDisBlock.contains(currentDisBlock))
                        word->availableDisBlock.append(currentDisBlock);
                    if (!word->availablePositions.contains(index))
                        word->availablePositions.append(index);
                    break;
                } else {
                    currentDisBlock++;
                }
            }
        }
        m_example->words.append(word);
    }

    CJsonManager::saveToFile(saveFileName, m_example);

    ui->pb_further->setEnabled(false);
    m_savedDiagnosis->clear();
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
    ui->lb_diagnosisCount->setText("Сохранено вариаций диагноза: " + QString::number(m_savedDiagnosis->count()));
    m_savingAccepter->close();
    m_menu->exec();
    _menuClosed();
}

void MainWindow::on_pb_further_clicked()
{
    m_savingAccepter->prepare();
    m_savingAccepter->setModal(true);
    m_savingAccepter->exec();

    ui->lb_diagnosisCount->setText("Сохранено вариаций диагноза: " + QString::number(m_savedDiagnosis->count()));
}

void MainWindow::on_pb_saveDiagnosis_clicked()
{
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

    QList<QStringList> diagnosis;
    for (auto lw : m_listWidgets) {
        auto words = new QStringList;
        for (auto i = 0; i < lw->count(); i++) {
            words->append(lw->item(i)->text());
        }
        diagnosis.append(*words);
    }

    // WARNING: подмена
    diagnosis = allWords;

    m_savedDiagnosis->append(diagnosis);
    ui->lb_diagnosisCount->setText("Сохранено вариаций диагноза: " + QString::number(m_savedDiagnosis->count()));

    if (!ui->pb_further->isEnabled())
        ui->pb_further->setEnabled(true);
}

void MainWindow::closeApp()
{
    qApp->quit();
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
            // TODO: СЕЙЧАС
            _prepareSavedDiagnosisFromLoadedFile(&example);
            _setWordsInLws(&example);
        }
    } break;
    default:
        // NOTE: плохой костыль, нужно переделать
        QTimer::singleShot(50, this, &MainWindow::closeApp);
    }
}

void MainWindow::_prepareSavedDiagnosisFromLoadedFile(const SExample *example)
{
    // TODO: СЕЙЧАС заполнить m_savedDiagnosis проверив все ли варианты правильно указались (количество, порядок слов)
    //наверное с комбинациями надо отработать

    // 1. Получить список всех возможных комбинаций
    QList<qint32> variationsForWord;
    for (auto word : example->words) {
        variationsForWord.append(word->availablePositions.count());
    }
    qDebug() << variationsForWord;
    /// Возможный алгоритм:
    /// Расставить слова по первой вариации -> если для первого слова есть альтернативы, то поставить на вторую позицию
    /// -> повторять по всем позициям для каждого слова
    // 2. Для каждой комбинации расставить по LW и сохранить эту комбинацию
}

void MainWindow::_setWordsInLws(const SExample *example)
{
    for (auto word : example->words) {
        m_listWidgets.at(word->availableDisBlock.first())->addItem(word->text);
    }
}

// TODO: справка: не нужно изменять слова после сохранения диагнозов - или учесть это
