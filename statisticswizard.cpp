#include <QtWidgets>

#include "statisticswizard.h"

size_t ROW_COUNT = 8;
size_t COLUMN_COUNT = 5;
int buNeYa = 2;

int genderId = -1, ageId = -1, intentId = -1;

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
//	setPage(Page_Conclusion, new ConclusionPage);

//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));
	setStartId(Page_Age);
	setButtonText(QWizard::NextButton, "İleri");
	setButtonText(QWizard::BackButton, "Geri");
	setButtonText(QWizard::FinishButton, "Bitir");
	setButtonText(QWizard::HelpButton, "Yardım");
	setButtonText(QWizard::CancelButton, "İptal");
#ifndef Q_OS_MAC
	setWizardStyle(ModernStyle);
//	setWizardStyle(ClassicStyle);
//	setWizardStyle(MacStyle);
#endif
//    setOption(HaveHelpButton, true);	// help butonunu aktifleştirmek için. (false demek daha iyi olabilir)
	setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

	connect(this, SIGNAL(finished(int)), this, SLOT(saveChoices(int)));
//	connect(this, &QWizard::customButtonClicked, this, &StatisticsWizard::close);
	setWindowTitle("Okuyucu İstatistik Programı");
}

void StatisticsWizard::saveChoices(int resultCode)
{
//	QWizardPage* agePage = page(Page_Age);
//	agePage->
//	qDebug() << result();
	if(resultCode == 1)	// finish'e basilinca 1 oluyor sanırım. daha fazla test ve/veya doc'a bakmam lazım
	{
		SaveEvent saveEvent(ageId, genderId, intentId);
		saveEvent.saveNow();
	}
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

	radioButtonsAge[0] = new QRadioButton("0-6");
	radioButtonsAge[1] = new QRadioButton("7-14");
	radioButtonsAge[2] = new QRadioButton("15-64");
	radioButtonsAge[3] = new QRadioButton("65 ve üstü");

	gridLayout->addWidget(radioButtonsAge[0], 0, 0);
	gridLayout->addWidget(radioButtonsAge[1], 0, 1);
	gridLayout->addWidget(radioButtonsAge[2], 1, 0);
	gridLayout->addWidget(radioButtonsAge[3], 1, 1);

	ageButtonGroup = new QButtonGroup;
	genderButtonGroup = new QButtonGroup;

	foreach (auto radioButton, radioButtonsGender)
	{
		genderButtonGroup->addButton(radioButton);
		connect(radioButton, &QAbstractButton::clicked, this, &AgePage::whichButtonHasCheckedGender);
	}

	foreach (auto radioButton, radioButtonsAge)
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
//bool AgePage::isComplete() const
//{

//}


IntentOfComingPage::IntentOfComingPage(QWidget *parent) : QWizardPage(parent)
{
//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

	topLabel = new QLabel(tr("Kütüphanemize hangi amaçla geldiğinizi seçin. Birden fazla işaretleyebilirsiniz."));
//	setSubTitle(tr("Kütüphanemize hangi amaçla geldiğinizi seçin. Birden fazla işaretleyebilirsiniz."));
	topLabel->setWordWrap(true);

	QGridLayout *gridLayout = new QGridLayout;
//	gridLayout->setHorizontalSpacing(4);
//	gridLayout->setVerticalSpacing(3);

	radioButtonsIntent[0] = new QRadioButton("Kitap/Dergi/Gazete Okumak");
	radioButtonsIntent[1] = new QRadioButton("Kitap Ödünç Almak");
	radioButtonsIntent[2] = new QRadioButton("Ders Çalışmak");
	radioButtonsIntent[3] = new QRadioButton("İnternet kullanmak");
	radioButtonsIntent[4] = new QRadioButton("Atölye/Etkinliklere katılmak");

//    radioButton->setChecked(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(topLabel);
//    layout->addWidget(registerRadioButton);
//    layout->addWidget(evaluateRadioButton);

	gridLayout->addWidget(radioButtonsIntent[0]);//, 0, 0);
	gridLayout->addWidget(radioButtonsIntent[1]);//, 0, 1);
	gridLayout->addWidget(radioButtonsIntent[2]);//, 1, 0);
	gridLayout->addWidget(radioButtonsIntent[3]);//, 1, 1);
	gridLayout->addWidget(radioButtonsIntent[4]);//, 1, 1);

	intentButtonGroup = new QButtonGroup;
	foreach (auto radioButton, radioButtonsIntent)
	{
		intentButtonGroup->addButton(radioButton);
//		connect(radioButton, &QAbstractButton::clicked, this, &IntentOfComingPage::isComplete);
		connect(radioButton, &QAbstractButton::clicked, this, &IntentOfComingPage::completeChanged);
	}

//	connect(this, SIGNAL(isComplete), this, SIGNAL(completeChanged));
//	connect(this, SIGNAL(selectionChanged()), this, SIGNAL(completeChanged));

//	foreach (auto radioButton, radioButtonsIntent)
//	{
//		intentButtonGroup->addButton(radioButton);
//		connect(radioButton, &QAbstractButton::clicked, this, &IntentOfComingPage::isComplete);
//	}

	layout->addLayout(gridLayout);
	setLayout(layout);
}

int IntentOfComingPage::nextId() const
{
//	return StatisticsWizard::Page_Conclusion;
	return -1;
}
bool IntentOfComingPage::isComplete() const
{
//	intentButtonGroup->
	intentId = intentButtonGroup->id(intentButtonGroup->checkedButton()) * (-1) - buNeYa;
	qDebug() << intentId;
	return ((intentId != -1) & (ageId != -1) & (genderId != -1)) ? true : false;
}

ConclusionPage::ConclusionPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(tr("Complete Your Registration"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

	bottomLabel = new QLabel;
	bottomLabel->setWordWrap(true);

	agreeCheckBox = new QCheckBox(tr("I agree to the terms of the license"));

	registerField("conclusion.agree*", agreeCheckBox);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(bottomLabel);
	layout->addWidget(agreeCheckBox);
	setLayout(layout);
}

int ConclusionPage::nextId() const
{
	return -1;
}

void ConclusionPage::initializePage()
{
	QString licenseText;

	licenseText = tr("<u>Upgrade License Agreement:</u> This software is licensed under the terms of your current license.");
	bottomLabel->setText(licenseText);
}

//void ConclusionPage::setVisible(bool visible)
//{
//	QWizardPage::setVisible(visible);

//	if (visible) {

//		wizard()->setButtonText(QWizard::CustomButton1, tr("&Print"));
//		wizard()->setOption(QWizard::HaveCustomButton1, true);
//		connect(wizard(), &QWizard::customButtonClicked, this, &ConclusionPage::printButtonClicked);

//	} else {
//		wizard()->setOption(QWizard::HaveCustomButton1, false);
//		disconnect(wizard(), &QWizard::customButtonClicked, this, &ConclusionPage::printButtonClicked);
//	}
//}

//void ConclusionPage::printButtonClicked()
//{

//}
