#ifndef AGEPAGE_H
#define AGEPAGE_H

#include <QButtonGroup>
#include <QRadioButton>
#include <QWizardPage>

namespace Ui { class AgePage; }

class AgePage : public QWizardPage
{
    Q_OBJECT
public:
    AgePage(QWidget *parent = nullptr);
    int nextId() const override;
private:
    Ui::AgePage *ui;
    QButtonGroup* ageButtonGroup, *genderButtonGroup;
private slots:
    void whichButtonHasCheckedAge();
    void whichButtonHasCheckedGender();
//	bool isComplete() const override;
};

#endif // AGEPAGE_H
