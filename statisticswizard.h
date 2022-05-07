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
	SaveEvent(int ageChoice, int genderChoice, int intentChoice);
	void saveNow();
private:
	QString openFile();
	int age, gender, intent = 0;
	std::vector<std::vector<int>> parseFile();
	std::vector<std::vector<int>> buildStatistics();
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
private slots:
	bool isComplete() const override;
};

class ConclusionPage : public QWizardPage
{
	Q_OBJECT
public:
	ConclusionPage(QWidget *parent = nullptr);

	void initializePage() override;
	int nextId() const override;
//	void setVisible(bool visible) override;

//private slots:
//	void printButtonClicked();

private:
	QLabel *bottomLabel;
	QCheckBox *agreeCheckBox;
};

#endif
