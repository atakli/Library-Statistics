#include "saveevent.h"

#include <QDir>

size_t ROW_COUNT = 8;
size_t COLUMN_COUNT = 5;
enum {NumCountLabelsRows = 5, NumCountLabelsCols = 8};

const QString SaveEvent::statisticsFilePath = QDir::homePath() + QDir::separator() + "kutuphaneIstatistikleri.csv";

void SaveEvent::initializeFile()
{
    QFile file(statisticsFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << __func__ << " fonksiyonundaki dosya acilamadi";
        return;							// TODO: buraya girerse ne olcak?
    }

    QTextStream out(&file);
    for (int i = 0; i < NumCountLabelsCols; ++i)
    {
        for (int j = 0; j < NumCountLabelsRows; ++j)
            out << "0" << ",";
        out << 0 << '\n';
    }
    file.close();
}

int SaveEvent::findIndex(std::vector<int> genderVector, std::vector<int> ageVector)
{
    auto itAge = std::find(ageVector.begin(), ageVector.end(), 1);
    int indexAge = itAge - ageVector.begin();
    auto itGender = std::find(genderVector.begin(), genderVector.end(), 1);
    int indexGender = itGender - genderVector.begin();
    if(indexGender == 1)
        return (indexAge + 1) * 2 - 1;
    return (indexAge + 1) * 2 - 2;
}
int SaveEvent::findIndex(int indexGender, int indexAge)
{
    if(indexGender == 1)
        return (indexAge + 1) * 2 - 1;
    return (indexAge + 1) * 2 - 2;
}

SaveEvent::SaveEvent(int ageChoice, int genderChoice, int intentChoice) : age(ageChoice), gender(genderChoice), intent(intentChoice) {}

std::vector<std::vector<int>> operator+(const std::vector<std::vector<int>>& onceki, const std::vector<std::vector<int>>& sonraki)
{
    std::vector<std::vector<int> > sonuc( ROW_COUNT, std::vector<int>(COLUMN_COUNT));
    for(size_t i = 0; i < ROW_COUNT; ++i)
        for(size_t j = 0; j < COLUMN_COUNT; ++j)
            sonuc[i][j] = onceki[i][j] + sonraki[i][j];
    return sonuc;
}

QString SaveEvent::openFile()
{
    if(!QFile::exists(statisticsFilePath))
    {
        initializeFile();
        qDebug() << "girdi";
    }

    QFile file(statisticsFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << __func__ << " fonksiyonundaki dosya acilamadi";
        return "";							// TODO: buraya girerse ne olcak?
    }
    QString text = file.readAll();
    file.close();
    return text;
}
std::vector<std::vector<int>> SaveEvent::newStatistics()
{
    std::vector<std::vector<int>> statistics(ROW_COUNT, std::vector<int>(COLUMN_COUNT));
    int index = findIndex(gender, age);
    std::vector<int> intentVector(COLUMN_COUNT);
    intentVector[intent] = 1;
    statistics[index] = intentVector;
//	qDebug() << statistics[index];
    return statistics;
}
void SaveEvent::saveNow()
{
    auto statisticstoBeSaved = filedStatistics() + newStatistics();

    QFile file(statisticsFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << __func__ << " fonksiyonundaki dosya acilamadi";
        return;							// TODO: buraya girerse ne olcak?
    }

    QTextStream out(&file);
    foreach(const std::vector<int>& numbers, statisticstoBeSaved)
    {
        auto it = numbers.begin();
        for(; it != numbers.end() - 1; ++it)
        {
            out << *it << ",";
        }
        out << *it << '\n';
    }
    file.close();
}
std::vector<std::vector<int>> SaveEvent::filedStatistics()
{
    QStringList lines = openFile().split('\n');

    std::vector<std::vector<int>> statistics;
    std::vector<int> sonuc;

    foreach(QString line, lines)
    {
        QStringList elemanlar = line.split(',');
        foreach(auto eleman, elemanlar)
            sonuc.emplace_back(eleman.toInt());
        statistics.emplace_back(sonuc);
        sonuc.clear();
    }
    return statistics;
}
