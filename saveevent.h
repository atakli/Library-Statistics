#ifndef SAVEEVENT_H
#define SAVEEVENT_H

#include <QString>

class SaveEvent
{
public:
//    friend std::vector<std::vector<int>> operator+(const std::vector<std::vector<int>>& onceki, const std::vector<std::vector<int>>& sonraki);
    SaveEvent(int ageChoice, int genderChoice, int intentChoice);
    void saveNow();
private:
    static QString openFile();
    static void initializeFile();
    static const QString statisticsFilePath;
    int age, gender, intent = 0;
    int findIndex(std::vector<int> genderVector, std::vector<int> ageVector);
    int findIndex(int indexGender, int indexAge);
    std::vector<std::vector<int>> filedStatistics();
    std::vector<std::vector<int>> newStatistics();
};

#endif // SAVEEVENT_H
