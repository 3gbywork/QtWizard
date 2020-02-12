#include "qtwizard.h"
#include "qtcustomwizard.h"
#include "qtcustomwizardpage.h"

#include <QObject>
#include <QWizard>
#include <QMap>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>

struct StWizardPage
{
    StWizardPage(){}
    StWizardPage(int id, QString title, QString subTitle)
    {
        this->id = id;
        this->title = title;
        this->subTitle = subTitle;
    }

    int id;
    QString title;
    QString subTitle;
};

class QtWizardHeader : public QWidget
{
public:
    QtWizardHeader(QMap<int, StWizardPage> *pages, QWidget *parent = nullptr);
    ~QtWizardHeader(){}

public slots:
    void wizardChanged(int currentId);

protected:
    void paintEvent(QPaintEvent *event);

private:
    const int HEADER_HEIGHT = 30;
//    const int HEADER_MIN_WIDTH = 1300;
    int m_currentId = 0;
    QMap<int, StWizardPage> *m_pages;
};

QtWizardHeader::QtWizardHeader(QMap<int, StWizardPage> *pages, QWidget *parent)
    :QWidget (parent)
{
    setMaximumHeight(HEADER_HEIGHT);
    setMinimumHeight(HEADER_HEIGHT);

//    setMinimumWidth(HEADER_MIN_WIDTH);

    m_pages = pages;
}

void QtWizardHeader::wizardChanged(int currentId)
{
    m_currentId = currentId;
    update();
}

void QtWizardHeader::paintEvent(QPaintEvent *event)
{
    auto palette = this->palette();

    auto contentRect = event->rect().adjusted(contentsMargins().left(), contentsMargins().top(), -contentsMargins().right(), -contentsMargins().bottom());
    auto count = m_pages->count();
    auto itemWidth = contentRect.width()/count;
    auto itemHeight = contentRect.height();
    auto sideLength = itemHeight/2;

    auto top = contentRect.top();
    auto right = contentRect.right();
    auto bottom = contentRect.bottom();
    auto left = contentRect.left();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(event->rect(), palette.color(QPalette::Background));

    int index = 0;
    int space = 5;
    int topBottomOffset = left-sideLength;
    int middleOffset = left+space;
    int length = itemWidth-space;
    QMapIterator<int, StWizardPage> iter(*m_pages);
    while (iter.hasNext())
    {
        iter.next();
        ++index;

        StWizardPage page = iter.value();

        QPainterPath path;
        if(1 == index)
        {
            path.moveTo(left, top);
            path.lineTo(topBottomOffset+length, top);
            path.lineTo(middleOffset+length, top+sideLength);
            path.lineTo(topBottomOffset+length, bottom);
            path.lineTo(left, bottom);
            path.lineTo(left, top);
        }
        else if(count == index)
        {
            path.moveTo(topBottomOffset, top);
            path.lineTo(right, top);
            path.lineTo(right, bottom);
            path.lineTo(topBottomOffset, bottom);
            path.lineTo(middleOffset, top+sideLength);
            path.lineTo(topBottomOffset, top);
        }
        else
        {
            path.moveTo(topBottomOffset, top);
            path.lineTo(topBottomOffset+length, top);
            path.lineTo(middleOffset+length, top+sideLength);
            path.lineTo(topBottomOffset+length, bottom);
            path.lineTo(topBottomOffset, bottom);
            path.lineTo(middleOffset, top+sideLength);
            path.lineTo(topBottomOffset, top);
        }

        //绘制边框
        QPen pen(Qt::white);
        pen.setWidth(2);
        painter.setBrush(palette.color(QPalette::Window));

        //填充图形
        if(page.id <= m_currentId)
        {
            painter.setBrush(QColor("#9effc3"));
        }
        painter.setPen(pen);
        painter.drawPath(path);

        //绘制文字
        QRect textRect(topBottomOffset, top, itemWidth+sideLength, itemHeight);
        auto color = palette.color(QPalette::WindowText);
        if(page.id <= m_currentId)
        {
            color = Qt::black;
        }

        //保存字体
        auto oldFont = painter.font();
        //调整标题字体大小
        auto titleWidth = painter.fontMetrics().horizontalAdvance(page.title);
        while(titleWidth > textRect.width() - sideLength*2)
        {
            auto curFont = painter.font();
            curFont.setPointSize(curFont.pointSize()-1);
            painter.setFont(curFont);

            titleWidth = painter.fontMetrics().horizontalAdvance(page.title);
        }
        painter.setPen(color);
        painter.drawText(textRect, Qt::AlignCenter, page.title);
        //恢复字体
        painter.setFont(oldFont);

        topBottomOffset += length;
        middleOffset += length;
    }
}

class QtWizardPrivate
{
public:
    QtWizardPrivate(QtWizard *p)
        :q_ptr(p)
    {

    }

public:
    void init()
    {
        auto parentWidget = q_func();
        auto layout = new QGridLayout(parentWidget);
        layout->setMargin(0);
        layout->setSpacing(0);

        mapWizardPage = new QMap<int, StWizardPage>();

        header = new QtWizardHeader(mapWizardPage);
        header->setContentsMargins(2, 2, 2, 2);
        layout->addWidget(header, 0, 0, 1, 1);

        wizard = new QtCustomWizard();
        layout->addWidget(wizard, 1, 0, 1, 1);

        auto updateHeader = [=]{header->wizardChanged(wizard->currentId());};
        QObject::connect(wizard, &QWizard::currentIdChanged, updateHeader);
        QObject::connect(wizard, &QWizard::pageAdded, updateHeader);
        QObject::connect(wizard, &QWizard::pageRemoved, updateHeader);
        QObject::connect(wizard, &QWizard::finished, parentWidget, &QtWizard::wizardFinished);
    }

    int addPage(QWizardPage *page)
    {
        auto id = wizard->addPage(page);
        auto info = StWizardPage(id, page->title(), page->subTitle());
        mapWizardPage->insert(id, info);
        //不清空会在导航条下显示
        page->setTitle("");
        page->setSubTitle("");
        return id;
    }

    int addPage(QWidget *page, QString title, QString subTitle, CompleteFunc completeFunc)
    {
        auto wizardPage = new QtCustomWizardPage(nullptr, completeFunc);
        wizardPage->setTitle(title);
        wizardPage->setSubTitle(subTitle);

        page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        page->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

        auto layout = new QGridLayout(wizardPage);
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(page, 0, 0, 1, 1);

        return addPage(wizardPage);
    }

    void removePage(int id)
    {
        wizard->removePage(id);
        mapWizardPage->remove(id);
    }

    QWizardPage* page(int id)
    {
        return wizard->page(id);
    }

private:
    QtWizard *q_ptr;
    Q_DECLARE_PUBLIC(QtWizard)

    QWizard *wizard;
    QMap<int, StWizardPage> *mapWizardPage;
    QtWizardHeader *header;
};

QtWizard::QtWizard(QWidget *parent)
    :QDialog (parent)
    ,dd_ptr(new QtWizardPrivate(this))
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    auto d = d_func();
    d->init();
}

int QtWizard::addPage(QWizardPage *page)
{
    auto d = d_func();
    return d->addPage(page);
}

int QtWizard::addPage(QWidget *page, QString title, QString subTitle, CompleteFunc completeFunc)
{
    auto d = d_func();
    return d->addPage(page, title, subTitle, completeFunc);
}

void QtWizard::removePage(int id)
{
    auto d = d_func();
    d->removePage(id);
}

QWizardPage* QtWizard::page(int id)
{
    auto d = d_func();
    return d->page(id);
}

void QtWizard::wizardFinished(int result)
{
    if (QDialog::Accepted == result)
        accept();
    reject();
}
