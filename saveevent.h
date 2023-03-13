#ifndef SAVEEVENT_H
#define SAVEEVENT_H

#include <QIODevice>
#include <QString>
#include <QFile>

class SaveEvent
{
public:
//    friend std::vector<std::vector<int>> operator+(const std::vector<std::vector<int>>& onceki, const std::vector<std::vector<int>>& sonraki);
    SaveEvent(int ageChoice, int genderChoice, int intentChoice);
    void saveNow();
private:
    static QString readFile();
    static void initializeFile();
    int age, gender, intent = 0;
    static std::unique_ptr<QFile> openFile(QIODevice::OpenMode mode, const char* func_name);
    int findIndex(std::vector<int> genderVector, std::vector<int> ageVector);
    int findIndex(int indexGender, int indexAge);
    std::vector<std::vector<int>> filedStatistics();
    std::vector<std::vector<int>> newStatistics();
};

#endif // SAVEEVENT_H
