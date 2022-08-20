#ifndef INTENTOFCOMINGPAGE_H
#define INTENTOFCOMINGPAGE_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QWizardPage>

namespace Ui { class WizardPage; }

class IntentOfComingPage : public QWizardPage
{
    Q_OBJECT
public:
    IntentOfComingPage(QWidget *parent = nullptr);
    int nextId() const override;
private:
    Ui::WizardPage *ui;
    QButtonGroup* intentButtonGroup;
    bool isComplete() const override;
};

#endif // INTENTOFCOMINGPAGE_H
