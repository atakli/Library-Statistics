#ifndef StatisticsWizard_H
#define StatisticsWizard_H

#include <QWizard>

QT_BEGIN_NAMESPACE
class QLabel;
class QRadioButton;
class QButtonGroup;
class QCheckBox;
QT_END_NAMESPACE

class SaveEvent
{
public:
//    friend std::vector<std::vector<int>> operator+(const std::vector<std::vector<int>>& onceki, const std::vector<std::vector<int>>& sonraki);
    SaveEvent(int ageChoice, int genderChoice, int intentChoice);
	void saveNow();
private:
	QString openFile();
    void initializeFile();
	int age, gender, intent = 0;
    std::vector<std::vector<int>> filedStatistics();
    std::vector<std::vector<int>> newStatistics();
};

class StatisticsWizard : public QWizard
{
    Q_OBJECT
public:
	enum { Page_Age, Page_Intent, Page_Conclusion};
	StatisticsWizard(QWidget *parent = nullptr);
private slots:
	void saveChoices(int resultCode);
};

class AgePage : public QWizardPage
{
    Q_OBJECT
public:
	AgePage(QWidget *parent = nullptr);
    int nextId() const override;
private:
	QLabel *topLabelGender, *topLabelAge;
	std::array<QRadioButton*, 4> radioButtonsAge;
	std::array<QRadioButton*, 2> radioButtonsGender;
	QButtonGroup* ageButtonGroup, *genderButtonGroup;
private slots:
	void whichButtonHasCheckedAge();
	void whichButtonHasCheckedGender();
//	bool isComplete() const override;
};

class IntentOfComingPage : public QWizardPage
{
    Q_OBJECT
public:
	IntentOfComingPage(QWidget *parent = nullptr);
    int nextId() const override;
private:
	QLabel *topLabel;
	std::array<QRadioButton*, 5> radioButtonsIntent;
	QButtonGroup* intentButtonGroup;
//private slots:
	bool isComplete() const override;
};

class ConclusionPage : public QWizardPage
{
	Q_OBJECT
public:
	ConclusionPage(QWidget *parent = nullptr);

	void initializePage() override;
	int nextId() const override;
private:
	QLabel *bottomLabel;
	QCheckBox *agreeCheckBox;
};

#endif
