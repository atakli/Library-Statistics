#ifndef CONCLUSIONPAGE_H
#define CONCLUSIONPAGE_H

class ConclusionPage : public QWizardPage
{
    Q_OBJECT
public:
    ConclusionPage(QWidget *parent = nullptr);

    void initializePage() override;
    int nextId() const override;
private:
    QLabel *bottomLabel;
    QCheckBox *agreeCheckBox;
};

#endif // CONCLUSIONPAGE_H
