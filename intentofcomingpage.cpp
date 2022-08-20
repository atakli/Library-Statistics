#include "intentofcomingpage.h"
#include "ui_intentofcomingpage.h"

#include <iostream>
#include <QDebug>

extern int buNeYa;
extern int genderId, ageId, intentId;

IntentOfComingPage::IntentOfComingPage(QWidget *parent) : QWizardPage(parent), ui(new Ui::WizardPage)
{
//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));
    ui->setupUi(this);
    intentButtonGroup = new QButtonGroup;
    foreach (auto radioButton, ui->groupBox_3->findChildren<QRadioButton*>(Qt::FindDirectChildrenOnly))
    {
        intentButtonGroup->addButton(radioButton);
        connect(radioButton, &QAbstractButton::clicked, this, &IntentOfComingPage::completeChanged);
    }
}

int IntentOfComingPage::nextId() const
{
    return -1;
}
bool IntentOfComingPage::isComplete() const
{
    intentId = intentButtonGroup->id(intentButtonGroup->checkedButton()) * (-1) - buNeYa;
    return intentId != -1 && ageId != -1 && genderId != -1;
}
