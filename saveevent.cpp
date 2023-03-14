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
    auto file = openFile(QIODevice::WriteOnly, __func__);

    const int number_of_bytes = NumCountLabelsRows * NumCountLabelsCols * sizeof(int);
    const QByteArray zeros{number_of_bytes, '\0'}; // 0 de ayni hesap. ama '0' farkli. // dongu ile yazmak daha performant diye dusunuyorum ama boyle daha okunakli sanki
    if (file->write(zeros) != number_of_bytes)
        qDebug() << "Failed to write zeros to file";
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
    std::unique_ptr<QFile> statistics = openFile(QIODevice::ReadWrite, __func__); // ah be. replace yapiyodum ama yaptigim noktadan sonrasi yok oluyordu. meger writeonly mode'da acmisim. cok tehlikeli!
    const qint64 pos = 2 * age + 1 * gender + (NumCountLabelsCols) * intent;
    if (!statistics->seek(pos * sizeof(int)))   // degistirilecek yere geliyorum
        qDebug() << "error in seek to read";

    const QByteArray number_old = statistics->read(1 * sizeof(int));
    if (!statistics->seek(pos * sizeof(int)))   // degistirilecek yere tekrar geliyorum
        qDebug() << "error in seek to write";

    const int new_number = [number_old]
    {
        if (std::endian::native == std::endian::big)
            return number_old.toInt() + 1;
        else
        {
            QDataStream stream(number_old);
            stream.setByteOrder(QDataStream::LittleEndian);
            int num;
            stream >> num; // read the integer from the data stream
            return num + 1;
        }
    }();
    const qint64 size = statistics->write((const char*)&new_number, 1 * sizeof(int));
    if (size != sizeof(int))
        qDebug() << "error in write";
    statistics->close();
}
