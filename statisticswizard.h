#ifndef StatisticsWizard_H
#define StatisticsWizard_H

#include <QWizard>

QT_BEGIN_NAMESPACE
class QLabel;
class QRadioButton;
class QButtonGroup;
QT_END_NAMESPACE

class SaveEvent
{
public:
	SaveEvent(int ageChoice, int genderChoice, int intentChoice);
	void saveNow();
private:
	QString openFile();
	int age, gender, intent;
	std::vector<std::vector<int>> parseFile();
	std::vector<std::vector<int>> buildStatistics();
};

class StatisticsWizard : public QWizard
{
    Q_OBJECT
public:
	enum { Page_Age, Page_Intent};
	StatisticsWizard(QWidget *parent = nullptr);
private slots:
	void saveChoices();
};

class AgePage : public QWizardPage
{
    Q_OBJECT
public:
	AgePage(QWidget *parent = nullptr);
    int nextId() const override;
private:
	QLabel *topLabelGender, *topLabelAge;
	std::array<QRadioButton*, 4> radioButtons;
	std::array<QRadioButton*, 2> radioButtonsGender;
	QButtonGroup* ageButtonGroup, *genderButtonGroup;
private slots:
	void whichButtonHasCheckedAge();
	void whichButtonHasCheckedGender();
};

class IntentOfComingPage : public QWizardPage
{
    Q_OBJECT

public:
	IntentOfComingPage(QWidget *parent = nullptr);
    int nextId() const override;
private:
	QLabel *topLabel;
	std::array<QRadioButton*, 5> radioButtons;
	QButtonGroup* intentButtonGroup;
private slots:
	void whichButtonHasChecked();
};

#endif
