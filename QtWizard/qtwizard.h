#ifndef QTWIZARD_H
#define QTWIZARD_H

#include "qtwizard_global.h"
#include "qtwizard_type.h"

#include <QDialog>
#include <QWizardPage>

class QtWizardPrivate;
class QTWIZARDSHARED_EXPORT QtWizard : public QDialog
{
    Q_OBJECT
public:
    QtWizard(QWidget *parent = nullptr);

public:
    ///
    /// \brief addPage  Adds the given page to the wizard, and returns the page's ID.
    /// \param page
    /// \return
    ///
    int addPage(QWizardPage *page);
    ///
    /// \brief addPage  Adds the given page to the wizard, and returns the page's ID.
    /// \param page
    /// \param title
    /// \param subTitle
    /// \return
    ///
    int addPage(QWidget *page, QString title = QString(), QString subTitle = QString(), CompleteFunc completeFunc = nullptr);
    ///
    /// \brief removePage   Removes the page with the given id.
    /// \param id
    ///
    void removePage(int id);
    ///
    /// \brief page Returns the page with the given id, or 0 if there is no such page.
    /// \param id
    /// \return
    ///
    QWizardPage* page(int id);

private slots:
    void wizardFinished(int result);

private:
    QtWizardPrivate *dd_ptr;
    Q_DECLARE_PRIVATE_D(dd_ptr, QtWizard)
};

#endif // QTWIZARD_H
