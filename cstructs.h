#ifndef CSTRUCTS_H
#define CSTRUCTS_H
#include <QStringList>

struct SWord {
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

enum class EBlockState { none, correct, incorrect, noDifference };

#endif // CSTRUCTS_H
