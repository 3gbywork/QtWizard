#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qtwizard.h"

#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    auto wizard = new QtWizard(this);
    wizard->addPage(new QTextEdit(), tr("test1"), tr("sub title"));
    wizard->addPage(new QTextEdit(), tr("test2"), tr("sub title"));
    wizard->addPage(new QTextEdit(), tr("test3"), tr("sub title"));

    auto txt = new QTextEdit();
    auto id = wizard->addPage(txt, tr("test4"), tr("sub title"), [=]()->bool{return !txt->toPlainText().isEmpty();});
    auto page = wizard->page(id);
    connect(txt, &QTextEdit::textChanged, page, &QWizardPage::completeChanged);

    auto page2 = new QWizardPage();
    page2->setTitle("test5");
    page2->setSubTitle("sub title");
    wizard->addPage(page2);

    wizard->show();
}
