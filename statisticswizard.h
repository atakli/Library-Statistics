#ifndef STATISTICSWIZARD_H
#define STATISTICSWIZARD_H

#include <QWizard>

class StatisticsWizard : public QWizard
{
    Q_OBJECT
public:
    enum { Page_Age, Page_Intent, Page_Conclusion};
    StatisticsWizard(QWidget *parent = nullptr);
private slots:
    void saveChoices(int resultCode);
};

#endif // STATISTICSWIZARD_H
