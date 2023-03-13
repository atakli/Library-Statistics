#include "agepage.h"
#include "ui_agepage.h"
#include <iostream>

int buNeYa = 2;
int genderId = -1, ageId = -1, intentId = -1;

AgePage::AgePage(QWidget *parent) : QWizardPage(parent), ui(new Ui::AgePage)
{
//	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));
    ui->setupUi(this);
//    std::cout << "count: " << ui->horizontalLayout_4->count() << std::endl;
    genderButtonGroup = new QButtonGroup;
    foreach (auto radioButton, ui->groupBox->findChildren<QRadioButton*>(Qt::FindDirectChildrenOnly))
    {
        genderButtonGroup->addButton(radioButton);
        connect(radioButton, &QAbstractButton::clicked, this, &AgePage::whichButtonHasCheckedGender);
    }

    ageButtonGroup = new QButtonGroup;
    foreach (auto radioButton, ui->groupBox_2->findChildren<QRadioButton*>(Qt::FindDirectChildrenOnly))
    {
        ageButtonGroup->addButton(radioButton);
        connect(radioButton, &QAbstractButton::clicked, this, &AgePage::whichButtonHasCheckedAge);
//        connect(radioButton, &QAbstractButton::clicked, this, [this](){ageId = ageButtonGroup->id(ageButtonGroup->checkedButton()) * (-1) - buNeYa;});  // TODO: daha sade olursa kalsın yoksa lambda olmasın
    }
}

void AgePage::whichButtonHasCheckedAge()
{
    ageId = ageButtonGroup->id(ageButtonGroup->checkedButton()) * (-1) - buNeYa;
//    qDebug() << "page age id:" << ageId;
}
void AgePage::whichButtonHasCheckedGender()
{
    genderId = genderButtonGroup->id(genderButtonGroup->checkedButton()) * (-1) - buNeYa;
}
int AgePage::nextId() const
{
    return 1;
}
//bool AgePage::isComplete() const
//{

//}
