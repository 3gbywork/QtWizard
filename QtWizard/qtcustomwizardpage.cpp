#include "qtcustomwizardpage.h"

QtCustomWizardPage::QtCustomWizardPage(QWidget *parent, CompleteFunc completeFunc) : QWizardPage(parent)
{
    this->completeFunc = completeFunc;
}

bool QtCustomWizardPage::isComplete() const
{
    if(nullptr == completeFunc)
        return QWizardPage::isComplete();
    return completeFunc();
}
