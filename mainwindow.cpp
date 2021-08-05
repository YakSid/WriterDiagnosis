#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "cjsonmanager.h"

const auto PROPERTY_BLOCK_TYPE = QByteArrayLiteral("blockType");

// TODO: на будущее: сделать хоткей для добавления фразы, например "+" по координатам курсора
// TODO: на будущее: сделать иконку программы и иконку для действия удаления в контекстном меню

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_savedDiagnosis = new QList<QList<QStringList>>;
    m_savingAccepter = new CSavingAccepter(m_savedDiagnosis, this);

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
}

MainWindow::~MainWindow()
{
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
}
