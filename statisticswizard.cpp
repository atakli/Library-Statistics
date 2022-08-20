#include "intentofcomingpage.h"
#include "statisticswizard.h"
#include "saveevent.h"
#include "agepage.h"

#include <iostream>

extern int genderId, ageId, intentId;

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
    if(resultCode == QDialog::Accepted)	// finish'e basilinca 1 oluyor sanırım. daha fazla test ve/veya doc'a bakmam lazım
    {
        SaveEvent saveEvent(ageId, genderId, intentId);
        saveEvent.saveNow();
    }
}


