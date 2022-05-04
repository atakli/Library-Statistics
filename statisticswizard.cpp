#include <QtWidgets>

#include "statisticswizard.h"

QString emailRegExp = QStringLiteral(".+@.+");
size_t ROW_COUNT = 8;
size_t COLUMN_COUNT = 5;
int buNeYa = 2;

int genderId, ageId, intentId;

int findIndex(std::vector<int> genderVector, std::vector<int> ageVector)
{
	auto itAge = std::find(ageVector.begin(), ageVector.end(), 1);
	int indexAge = itAge - ageVector.begin();
	auto itGender = std::find(genderVector.begin(), genderVector.end(), 1);
	int indexGender = itGender - genderVector.begin();
	if(indexGender == 1)
		return (indexAge + 1) * 2 - 1;
	return (indexAge + 1) * 2 - 2;
}
int findIndex(int indexGender, int indexAge)
{
	if(indexGender == 1)
		return (indexAge + 1) * 2 - 1;
	return (indexAge + 1) * 2 - 2;
}

SaveEvent::SaveEvent(int ageChoice, int genderChoice, int intentChoice) : age(ageChoice), gender(genderChoice), intent(intentChoice) {}

std::vector<std::vector<int>> topla(std::vector<std::vector<int>>& onceki, std::vector<std::vector<int>>& sonraki)
{
	std::vector<std::vector<int> > sonuc( ROW_COUNT, std::vector<int>(COLUMN_COUNT));
	for(size_t i = 0; i < ROW_COUNT; ++i)
		for(size_t j = 0; j < COLUMN_COUNT; ++j)
			sonuc[i][j] = onceki[i][j] + sonraki[i][j];
	return sonuc;
}

QString SaveEvent::openFile()
{
	QFile file("istatistikler.csv");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";							// TODO: buraya girerse ne olcak?
	QString text = file.readAll();
	file.close();
	return text;
}
std::vector<std::vector<int>> SaveEvent::buildStatistics()
{
	std::vector<std::vector<int>> statistics(ROW_COUNT, std::vector<int>(COLUMN_COUNT));
	int index = findIndex(gender, age);
	std::vector<int> intentVector(COLUMN_COUNT);
	intentVector[intent] = 1;
	statistics[index] = intentVector;
//	qDebug() << statistics[index];
	return statistics;
}
void SaveEvent::saveNow()
{
	std::vector<std::vector<int>> newStatistics = buildStatistics();
	std::vector<std::vector<int>> savedStatistics = parseFile();

	std::vector<std::vector<int>> statisticstoBeSaved = topla(savedStatistics, newStatistics);

	QFile file("istatistikler.csv");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;							// TODO: buraya girerse ne olcak?

	QTextStream out(&file);
	foreach(std::vector<int> numbers, statisticstoBeSaved)
	{
		auto it = numbers.begin();
		for(; it != numbers.end() - 1; ++it)
		{
			out << *it << ",";
		}
		out << *it << '\n';
	}
	file.close();
}
std::vector<std::vector<int>> SaveEvent::parseFile()
{
	QStringList lines = openFile().split('\n');

//	std::vector<std::vector<int>> statistics(ROW_COUNT, std::vector<int>(COLUMN_COUNT));
	std::vector<std::vector<int>> statistics;
	std::vector<int> sonuc;

	foreach(QString line, lines)
	{
//		qDebug() << "line:" << line;
		QStringList elemanlar = line.split(',');
		foreach(auto eleman, elemanlar)
			sonuc.emplace_back(eleman.toInt());
		statistics.emplace_back(sonuc);
//		qDebug() << sonuc;
		sonuc.clear();
	}
//	qDebug() << statistics.size();
//	foreach(auto k1, statistics)
//	{
//		foreach(auto k2, k1)
//			qDebug() << k2;
//		qDebug() << "da";
//	}
	return statistics;
}

StatisticsWizard::StatisticsWizard(QWidget *parent) : QWizard(parent)
{
	setPage(Page_Age, new AgePage);
	setPage(Page_Intent, new IntentOfComingPage);

//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));
	setStartId(Page_Age);
	setButtonText(QWizard::NextButton, "İleri");
	setButtonText(QWizard::BackButton, "Geri");
	setButtonText(QWizard::FinishButton, "Bitir");
	setButtonText(QWizard::HelpButton, "Yardım");
	setButtonText(QWizard::CancelButton, "İptal");
#ifndef Q_OS_MAC
	setWizardStyle(ModernStyle);
#endif
//    setOption(HaveHelpButton, true);	// help butonunu aktifleştirmek için. (false demek daha iyi olabilir)
	setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

	connect(this, &QWizard::finished, this, &StatisticsWizard::saveChoices);
}

void StatisticsWizard::saveChoices()
{
//	QWizardPage* agePage = page(Page_Age);
//	agePage->
	SaveEvent saveEvent(ageId, genderId, intentId);
	saveEvent.saveNow();
}

AgePage::AgePage(QWidget *parent) : QWizardPage(parent)
{
	setTitle("Kütüphanemize hoşgeldiniz");
//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

	topLabelGender = new QLabel(tr("Cinsiyet seçin"));
	topLabelGender->setAlignment(Qt::AlignHCenter);
	topLabelGender->setWordWrap(true);
	topLabelAge = new QLabel(tr("Yaş aralığınızı seçin"));
	topLabelAge->setAlignment(Qt::AlignHCenter);
	topLabelAge->setWordWrap(true);

	QGridLayout *gridLayout = new QGridLayout;
	QGridLayout *gridLayoutGender = new QGridLayout;
	gridLayout->setAlignment(Qt::AlignHCenter);
	gridLayoutGender->setAlignment(Qt::AlignHCenter);

	radioButtonsGender[0] = new QRadioButton("Erkek");
	radioButtonsGender[1] = new QRadioButton("Kadın");

	gridLayoutGender->addWidget(radioButtonsGender[0], 0, 0);
	gridLayoutGender->addWidget(radioButtonsGender[1], 0, 1);

	radioButtons[0] = new QRadioButton("0-6");
	radioButtons[1] = new QRadioButton("7-14");
	radioButtons[2] = new QRadioButton("15-64");
	radioButtons[3] = new QRadioButton("65 ve üstü");

	gridLayout->addWidget(radioButtons[0], 0, 0);
	gridLayout->addWidget(radioButtons[1], 0, 1);
	gridLayout->addWidget(radioButtons[2], 1, 0);
	gridLayout->addWidget(radioButtons[3], 1, 1);

	ageButtonGroup = new QButtonGroup;
	genderButtonGroup = new QButtonGroup;

	foreach (auto radioButton, radioButtonsGender)
	{
		genderButtonGroup->addButton(radioButton);
		connect(radioButton, &QAbstractButton::clicked, this, &AgePage::whichButtonHasCheckedGender);
	}

	foreach (auto radioButton, radioButtons)
	{
		ageButtonGroup->addButton(radioButton);
		connect(radioButton, &QAbstractButton::clicked, this, &AgePage::whichButtonHasCheckedAge);
	}

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(topLabelGender);
	layout->addLayout(gridLayoutGender);
	layout->addSpacing(30);
	layout->addWidget(topLabelAge);
	layout->addLayout(gridLayout);
    setLayout(layout);
}

int AgePage::nextId() const
{
	return StatisticsWizard::Page_Intent;
}
void AgePage::whichButtonHasCheckedAge()
{
	ageId = ageButtonGroup->id(ageButtonGroup->checkedButton()) * (-1) - buNeYa;
}
void AgePage::whichButtonHasCheckedGender()
{
	genderId = genderButtonGroup->id(genderButtonGroup->checkedButton()) * (-1) - buNeYa;
}


IntentOfComingPage::IntentOfComingPage(QWidget *parent) : QWizardPage(parent)
{
//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

	topLabel = new QLabel(tr("Kütüphanemize hangi amaçla geldiğinizi seçin. Birden fazla işaretleyebilirsiniz."));
//	setSubTitle(tr("Kütüphanemize hangi amaçla geldiğinizi seçin. Birden fazla işaretleyebilirsiniz."));
	topLabel->setWordWrap(true);

	QGridLayout *gridLayout = new QGridLayout;
//	gridLayout->setHorizontalSpacing(4);
//	gridLayout->setVerticalSpacing(3);

	radioButtons[0] = new QRadioButton("Kitap/Dergi/Gazete Okumak");
	radioButtons[1] = new QRadioButton("Kitap Ödünç Almak");
	radioButtons[2] = new QRadioButton("Ders Çalışmak");
	radioButtons[3] = new QRadioButton("İnternet kullanmak");
	radioButtons[4] = new QRadioButton("Etkinliklere katılmak");

//    radioButton->setChecked(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(topLabel);
//    layout->addWidget(registerRadioButton);
//    layout->addWidget(evaluateRadioButton);

	gridLayout->addWidget(radioButtons[0]);//, 0, 0);
	gridLayout->addWidget(radioButtons[1]);//, 0, 1);
	gridLayout->addWidget(radioButtons[2]);//, 1, 0);
	gridLayout->addWidget(radioButtons[3]);//, 1, 1);
	gridLayout->addWidget(radioButtons[4]);//, 1, 1);

	intentButtonGroup = new QButtonGroup;
	foreach (auto radioButton, radioButtons)
	{
		intentButtonGroup->addButton(radioButton);
		connect(radioButton, &QAbstractButton::clicked, this, &IntentOfComingPage::whichButtonHasChecked);
	}

	layout->addLayout(gridLayout);
	setLayout(layout);
}

int IntentOfComingPage::nextId() const
{
	return -1;
}
void IntentOfComingPage::whichButtonHasChecked()
{
	intentId = intentButtonGroup->id(intentButtonGroup->checkedButton()) * (-1) - buNeYa;
}
