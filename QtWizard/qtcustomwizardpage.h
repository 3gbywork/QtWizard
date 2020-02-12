#ifndef QTCUSTOMWIZARDPAGE_H
#define QTCUSTOMWIZARDPAGE_H

#include "qtwizard_type.h"

#include <QWizardPage>

class QtCustomWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit QtCustomWizardPage(QWidget *parent = nullptr, CompleteFunc completeFunc = nullptr);

public:
    bool isComplete() const;

signals:

public slots:

private:
    CompleteFunc completeFunc;
};

#endif // QTCUSTOMWIZARDPAGE_H
