#include "saveevent.h"

#include <QMessageBox>
#include <QDir>

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
    for (int i = 0; i < NumCountLabelsRows; ++i)
    {
        for (int j = 0; j < NumCountLabelsCols - 1; ++j)
        {
            out << "0,";
        }
        out << "0\n";
    }
    file->close();
}

SaveEvent::SaveEvent(int ageChoice, int genderChoice, int intentChoice) : age(ageChoice), gender(genderChoice), intent(intentChoice)
{
    if(!QFile::exists(statisticsFile))
    {
        initializeFile();
        qDebug() << statisticsFile << "yoktu olusturduk";
    }
}

void SaveEvent::saveNow()
{
    std::unique_ptr<QFile> statistics = openFile(QIODevice::ReadWrite | QIODevice::Text, __func__); // ah be. replace yapiyodum ama yaptigim noktadan sonrasi yok oluyordu. meger writeonly mode'da acmisim. cok tehlikeli!
    if (!statistics->seek(4 * age + 2 * gender + (NumCountLabelsCols + 7) * intent))   // degistirilecek yere geliyorum
        qDebug() << "error in seek";
    if (statistics->write("1", 1) != 1)
        qDebug() << "error in write";
    statistics->close();
}
