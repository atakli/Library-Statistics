#ifndef SAVEEVENT_H
#define SAVEEVENT_H

#include <QIODevice>
#include <QString>
#include <QFile>

#include <memory>

class SaveEvent
{
public:
//    friend std::vector<std::vector<int>> operator+(const std::vector<std::vector<int>>& onceki, const std::vector<std::vector<int>>& sonraki);
    SaveEvent(int ageChoice, int genderChoice, int intentChoice);
    void saveNow();
private:
    static void initializeFile();
    int age, gender, intent;
    static std::unique_ptr<QFile> openFile(QIODevice::OpenMode mode, const char* func_name);
};

#endif // SAVEEVENT_H
