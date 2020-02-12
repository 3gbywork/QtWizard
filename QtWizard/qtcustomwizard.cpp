#include "qtcustomwizard.h"

QtCustomWizard::QtCustomWizard(QWidget *parent) : QWizard(parent)
{
    setWizardStyle(QWizard::ModernStyle);
    setOption(QWizard::NoBackButtonOnStartPage);
}
