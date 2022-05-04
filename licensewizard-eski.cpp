/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "licensewizard.h"

QString emailRegExp = QStringLiteral(".+@.+");
size_t ROW_COUNT = 8;
size_t COLUMN_COUNT = 5;

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

QString SaveEvent::openFile(QIODevice::OpenModeFlag flag)
{
	QFile file("istatistikler.csv");
	if (!file.open(flag | QIODevice::Text))
		return "";							// TODO: buraya girerse ne olcak?
	QString text = file.readAll();
	file.close();
	return text;
}

void SaveEvent::saveNow()
{
	std::vector<std::vector<int>> statistics(ROW_COUNT,std::vector<int>(COLUMN_COUNT));
	std::vector<std::vector<int>> savedStatistics = parseFile();
	int index = findIndex(gender, age);
	std::vector<int> intentVector(COLUMN_COUNT);
//	qDebug() << age;
//	qDebug() << gender;
//	qDebug() << intent;
	intentVector[intent] = 1;
//	statistics[index] = intentVector;
//	qDebug() << statistics[index];
}
std::vector<std::vector<int>> SaveEvent::parseFile()
{
	QString text = openFile();

	QStringList lines = text.split('\n');
	std::string line;

	std::vector<std::vector<int>> istatistikler(ROW_COUNT, std::vector<int>(COLUMN_COUNT));
	std::vector<int> sonuc;

	foreach(QString line, lines)
	{
		QStringList elemanlar = line.split(',');
		foreach(auto eleman, elemanlar)
			sonuc.emplace_back(eleman.toInt());
		istatistikler.emplace_back(sonuc);
		sonuc.clear();
	}
	return istatistikler;
}

LicenseWizard::LicenseWizard(QWidget *parent) : QWizard(parent)
{
//! [0]
//	setPage(Page_Gender, new GenderPage);
	setPage(Page_Age, new AgePage);
	setPage(Page_Intent, new IntentOfComingPage);
//    setPage(Page_Register, new RegisterPage);
//    setPage(Page_Details, new DetailsPage);
//    setPage(Page_Conclusion, new ConclusionPage);
//! [1]

//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));
	setStartId(Page_Age);
//! [2]
	setButtonText(QWizard::NextButton, "İleri");
	setButtonText(QWizard::BackButton, "Geri");
	setButtonText(QWizard::FinishButton, "Bitir");
	setButtonText(QWizard::HelpButton, "Yardım");
	setButtonText(QWizard::CancelButton, "İptal");
//! [3]
#ifndef Q_OS_MAC
//! [3] //! [4]
    setWizardStyle(ModernStyle);
#endif
//! [4] //! [5]
//    setOption(HaveHelpButton, true);	// help butonunu aktifleştirmek için. (false demek daha iyi olabilir)
//! [5] //! [6]
	setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo.png"));

//! [7]
	connect(this, &QWizard::helpRequested, this, &LicenseWizard::showHelp);
	connect(this, &QWizard::finished, this, &LicenseWizard::saveChoices);
//! [7]

	setWindowTitle(tr(" "));
//! [8]
}
//! [6] //! [8]

//! [9] //! [10]
void LicenseWizard::showHelp()
//! [9] //! [11]
{
    static QString lastHelpMessage;

    QString message;

    switch (currentId()) {
	case Page_Age:
		message = tr("The decision you make here will affect which page you get to see next.");
        break;
//! [10] //! [11]
	case Page_Intent:
		message = tr("Make sure to provide a valid email address, such as toni.buddenbrook@example.de.");
        break;
    case Page_Register:
		message = tr("If you don't provide an upgrade key, you will be asked to fill in your details.");
        break;
    case Page_Details:
		message = tr("Make sure to provide a valid email address, such as thomas.gradgrind@example.co.uk.");
        break;
    case Page_Conclusion:
		message = tr("You must accept the terms and conditions of the license to proceed.");
        break;
//! [12] //! [13]
    default:
        message = tr("This help is likely not to be of any help.");
    }
//! [12]

    if (lastHelpMessage == message)
		message = tr("Sorry, I already gave what help I could. Maybe you should try asking a human?");

//! [14]
    QMessageBox::information(this, tr("License Wizard Help"), message);
//! [14]

    lastHelpMessage = message;
//! [15]
}
void LicenseWizard::saveChoices()
{
//	QWizardPage* agePage = page(Page_Age);
//	agePage->
//	qDebug() << ageChoice;
//	qDebug() << genderChoice;
//	qDebug() << intentChoice;
//	qDebug() << ageId;
//	qDebug() << genderId;
//	qDebug() << intentId;
	SaveEvent saveEvent(ageId, genderId, intentId);
	saveEvent.saveNow();
}
//! [13] //! [15]

//! [16]
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
//! [16] //! [17]

//! [18]
int AgePage::nextId() const
//! [17] //! [19]
{
	return LicenseWizard::Page_Intent;
}
void AgePage::whichButtonHasCheckedAge()
{
	ageChoice = ageButtonGroup->checkedButton()->text();
	ageId = ageButtonGroup->id(ageButtonGroup->checkedButton());
	qDebug() << "ageId:" << ageId;
}
void AgePage::whichButtonHasCheckedGender()
{
	genderChoice = genderButtonGroup->checkedButton()->text();
	genderId = genderButtonGroup->id(genderButtonGroup->checkedButton());
	qDebug() << "genderId:" << genderId;
}

//! [18] //! [19]

//! [20]
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
//! [22]

//! [23]
int IntentOfComingPage::nextId() const
{
//	return LicenseWizard::Page_Register;
	return -1;
}
void IntentOfComingPage::whichButtonHasChecked()
{
	intentChoice = intentButtonGroup->checkedButton()->text();
	intentId = intentButtonGroup->id(intentButtonGroup->checkedButton()) + 2;
	qDebug() << "intentId" << intentId;
}

RegisterPage::RegisterPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Register Your Copy of <i>Super Product One</i>&trade;"));
	setSubTitle(tr("If you have an upgrade key, please fill in the appropriate field."));

    nameLabel = new QLabel(tr("N&ame:"));
    nameLineEdit = new QLineEdit;
    nameLabel->setBuddy(nameLineEdit);

    upgradeKeyLabel = new QLabel(tr("&Upgrade key:"));
    upgradeKeyLineEdit = new QLineEdit;
    upgradeKeyLabel->setBuddy(upgradeKeyLineEdit);

    registerField("register.name*", nameLineEdit);
    registerField("register.upgradeKey", upgradeKeyLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(upgradeKeyLabel, 1, 0);
    layout->addWidget(upgradeKeyLineEdit, 1, 1);
    setLayout(layout);
}

//! [24]
int RegisterPage::nextId() const
{
    if (upgradeKeyLineEdit->text().isEmpty()) {
        return LicenseWizard::Page_Details;
    } else {
        return LicenseWizard::Page_Conclusion;
    }
}
//! [24]

DetailsPage::DetailsPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Fill In Your Details"));
	setSubTitle(tr("Please fill all three fields. Make sure to provide a valid email address (e.g., tanaka.aya@example.co.jp)."));

    companyLabel = new QLabel(tr("&Company name:"));
    companyLineEdit = new QLineEdit;
    companyLabel->setBuddy(companyLineEdit);

    emailLabel = new QLabel(tr("&Email address:"));
    emailLineEdit = new QLineEdit;
    emailLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp), this));
    emailLabel->setBuddy(emailLineEdit);

    postalLabel = new QLabel(tr("&Postal address:"));
    postalLineEdit = new QLineEdit;
    postalLabel->setBuddy(postalLineEdit);

    registerField("details.company*", companyLineEdit);
    registerField("details.email*", emailLineEdit);
    registerField("details.postal*", postalLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(companyLabel, 0, 0);
    layout->addWidget(companyLineEdit, 0, 1);
    layout->addWidget(emailLabel, 1, 0);
    layout->addWidget(emailLineEdit, 1, 1);
    layout->addWidget(postalLabel, 2, 0);
    layout->addWidget(postalLineEdit, 2, 1);
    setLayout(layout);
}

//! [25]
int DetailsPage::nextId() const
{
    return LicenseWizard::Page_Conclusion;
}
//! [25]

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
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

//! [26]
int ConclusionPage::nextId() const
{
    return -1;
}
//! [26]

//! [27]
void ConclusionPage::initializePage()
{
    QString licenseText;

	if (wizard()->hasVisitedPage(LicenseWizard::Page_Intent)) {
        licenseText = tr("<u>Evaluation License Agreement:</u> "
                         "You can use this software for 30 days and make one "
                         "backup, but you are not allowed to distribute it.");
    } else if (wizard()->hasVisitedPage(LicenseWizard::Page_Details)) {
        licenseText = tr("<u>First-Time License Agreement:</u> "
                         "You can use this software subject to the license "
                         "you will receive by email.");
    } else {
        licenseText = tr("<u>Upgrade License Agreement:</u> "
                         "This software is licensed under the terms of your "
                         "current license.");
    }
    bottomLabel->setText(licenseText);
}
//! [27]

//! [28]
void ConclusionPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);

    if (visible) {
//! [29]
        wizard()->setButtonText(QWizard::CustomButton1, tr("&Print"));
        wizard()->setOption(QWizard::HaveCustomButton1, true);
		connect(wizard(), &QWizard::customButtonClicked, this, &ConclusionPage::printButtonClicked);
//! [29]
    } else {
        wizard()->setOption(QWizard::HaveCustomButton1, false);
		disconnect(wizard(), &QWizard::customButtonClicked, this, &ConclusionPage::printButtonClicked);
    }
}
//! [28]

void ConclusionPage::printButtonClicked()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec())
		QMessageBox::warning(this, tr("Print License"), tr("As an environmentally friendly measure, the license text will not actually be printed."));
#endif
}
