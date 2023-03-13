#include "saveevent.h"

#include <QMessageBox>
#include <QDir>

#include <ranges>

const size_t ROW_COUNT = 8;
const size_t COLUMN_COUNT = 5;
enum {NumCountLabelsRows = 5, NumCountLabelsCols = 8};

const QString statisticsFile = QDir::homePath() + QDir::separator() + ".kutuphaneIstatistikleri.csv";
const QString appName = "Kütüphane İstatistik Programı";

std::unique_ptr<QFile> SaveEvent::openFile(QIODevice::OpenMode mode, const char* func_name) // ??? pointer'la yapmayinca neden copy ctor deleted hatasi verdi ???
{
    auto file = std::make_unique<QFile>(statisticsFile);
    if (!file->open(mode))
    {
        qDebug() << func_name << "fonksiyonundaki" << statisticsFile << "dosyasi acilamadi!";
        QMessageBox msgBox(QMessageBox::Question, appName, statisticsFile + " dosyasi acilamadi!", QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, "Tamam");
        msgBox.exec();
        exit(EXIT_FAILURE);
    }
    return file;
}

void SaveEvent::initializeFile()
{
    auto file = openFile(QIODevice::WriteOnly | QIODevice::Text, __func__);

    QTextStream out(file.get());
    for (int i = 0; i < NumCountLabelsCols; ++i)
    {
        for (int j = 0; j < NumCountLabelsRows - 1; ++j)
        {
            out << 0 << ",";
        }
        out << 0 << '\n';
    }
    file->close();
}

int SaveEvent::findIndex(std::vector<int> genderVector, std::vector<int> ageVector)
{
    const auto itAge = std::find(ageVector.begin(), ageVector.end(), 1);
    const int indexAge = itAge - ageVector.begin();
    const auto itGender = std::find(genderVector.begin(), genderVector.end(), 1);
    const int indexGender = itGender - genderVector.begin();
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

QString SaveEvent::readFile()
{
    if(!QFile::exists(statisticsFile))
    {
        initializeFile();
        qDebug() << statisticsFile << "yoktu olusturduk";
    }
    return openFile(QIODevice::ReadOnly | QIODevice::Text, __func__)->readAll();
}
std::vector<std::vector<int>> SaveEvent::newStatistics()
{
    std::vector<std::vector<int>> statistics(ROW_COUNT, std::vector<int>(COLUMN_COUNT));
    const int index = findIndex(gender, age);
    std::vector<int> intentVector(COLUMN_COUNT);
    intentVector[intent] = 1;
    statistics[index] = intentVector;
//	qDebug() << statistics[index];
    return statistics;
}
void SaveEvent::saveNow()
{
    const auto statisticstoBeSaved = filedStatistics() + newStatistics();

    auto file = openFile(QIODevice::WriteOnly | QIODevice::Text, __func__);

    QTextStream out(file.get());
    for (const std::vector<int>& numbers : statisticstoBeSaved)
    {
        auto it = numbers.begin();
        for(; it != numbers.end() - 1; ++it)
        {
            out << *it << ",";
        }
        out << *it << '\n';
    }
    file->close();
}
std::vector<std::vector<int>> SaveEvent::filedStatistics()
{
    const QStringList lines = readFile().split('\n');

    std::vector<std::vector<int>> statistics;

//    for (const QString& line : lines)
    std::ranges::transform(lines, std::back_inserter(statistics), [](const QString& line)
    {
        std::vector<int> sonuc;
        const QStringList elemanlar = line.split(',');
        std::ranges::transform(elemanlar, std::back_inserter(sonuc), [](const auto& eleman){return eleman.toInt();});
        return sonuc;
//        statistics.emplace_back(sonuc);
    });
    return statistics;
}
