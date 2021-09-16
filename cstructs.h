#ifndef CSTRUCTS_H
#define CSTRUCTS_H
#include <QStringList>

struct SWord {
    qint32 id;
    QString text;
    QList<qint8> availableDisBlock;
    qint8 currentDisBlock { -1 };
    QList<qint8> availablePositions;
    qint8 currentPosition { -1 };

    bool isCorrectPosition() { return availablePositions.contains(currentPosition); }
    bool isCorrectDisBlock() { return availableDisBlock.contains(currentDisBlock); }
    bool isCorrect() { return isCorrectPosition() && isCorrectDisBlock(); }
};

struct SExample {
    QStringList combinations; // {"1-2-3.4-5-6.7-8.9", "1-5-2.7-3-4.8-4.9", "2-1-4.3-5-4.5-6.9"};
    QList<SWord *> words;

    bool isCorrect()
    {
        for (auto word : words) {
            if (!word->isCorrect())
                return false;
        }
        return true;
    }
};

//! Разделяет ключи id слов в блоке
const auto SEPARATOR = QStringLiteral("-");
//! Разделяет блоки в комбинации
const auto BLOCK_SEPARATOR = QStringLiteral(".");

enum class EBlockState { none, correct, incorrect, noDifference };

#endif // CSTRUCTS_H
