/********************************************************************************
** Form generated from reading ui file 'qgsabout.ui'
**
** Created: Wed Jun 11 19:03:29 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QGSABOUT_H
#define UI_QGSABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_QgsAbout
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *Widget2;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout;
    QLabel *qgisIcon;
    QVBoxLayout *vboxLayout;
    QLabel *TextLabel4;
    QLabel *lblVersion;
    QTextEdit *lblUrls;
    QPushButton *btnQgisHome;
    QPushButton *btnQgisUser;
    QWidget *Widget3;
    QGridLayout *gridLayout2;
    QTextEdit *txtWhatsNew;
    QWidget *tab;
    QGridLayout *gridLayout3;
    QListWidget *listBox1;
    QFrame *frame;
    QGridLayout *gridLayout4;
    QLabel *pixAuthorMug;
    QLabel *textLabel1;
    QWidget *TabPage;
    QGridLayout *gridLayout5;
    QTextBrowser *txtBrowserPlugins;
    QWidget *tab_2;
    QGridLayout *gridLayout6;
    QTextEdit *txtSponsors;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *buttonCancel;

    void setupUi(QDialog *QgsAbout)
    {
    if (QgsAbout->objectName().isEmpty())
        QgsAbout->setObjectName(QString::fromUtf8("QgsAbout"));
    QgsAbout->resize(529, 375);
    QgsAbout->setSizeGripEnabled(true);
    gridLayout = new QGridLayout(QgsAbout);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    tabWidget = new QTabWidget(QgsAbout);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    Widget2 = new QWidget();
    Widget2->setObjectName(QString::fromUtf8("Widget2"));
    gridLayout1 = new QGridLayout(Widget2);
    gridLayout1->setSpacing(6);
    gridLayout1->setMargin(9);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    qgisIcon = new QLabel(Widget2);
    qgisIcon->setObjectName(QString::fromUtf8("qgisIcon"));
    qgisIcon->setMaximumSize(QSize(60, 60));
    qgisIcon->setPixmap(QPixmap(QString::fromUtf8("../../images/icons/qgis-icon-60x60.png")));
    qgisIcon->setScaledContents(false);

    hboxLayout->addWidget(qgisIcon);

    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    TextLabel4 = new QLabel(Widget2);
    TextLabel4->setObjectName(QString::fromUtf8("TextLabel4"));
    TextLabel4->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(TextLabel4);

    lblVersion = new QLabel(Widget2);
    lblVersion->setObjectName(QString::fromUtf8("lblVersion"));
    lblVersion->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(lblVersion);


    hboxLayout->addLayout(vboxLayout);


    gridLayout1->addLayout(hboxLayout, 0, 0, 1, 1);

    lblUrls = new QTextEdit(Widget2);
    lblUrls->setObjectName(QString::fromUtf8("lblUrls"));

    gridLayout1->addWidget(lblUrls, 3, 0, 1, 1);

    btnQgisHome = new QPushButton(Widget2);
    btnQgisHome->setObjectName(QString::fromUtf8("btnQgisHome"));
    btnQgisHome->setFlat(false);

    gridLayout1->addWidget(btnQgisHome, 2, 0, 1, 1);

    btnQgisUser = new QPushButton(Widget2);
    btnQgisUser->setObjectName(QString::fromUtf8("btnQgisUser"));
    btnQgisUser->setFlat(false);

    gridLayout1->addWidget(btnQgisUser, 1, 0, 1, 1);

    tabWidget->addTab(Widget2, QString());
    Widget3 = new QWidget();
    Widget3->setObjectName(QString::fromUtf8("Widget3"));
    gridLayout2 = new QGridLayout(Widget3);
    gridLayout2->setSpacing(6);
    gridLayout2->setMargin(9);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    txtWhatsNew = new QTextEdit(Widget3);
    txtWhatsNew->setObjectName(QString::fromUtf8("txtWhatsNew"));
    txtWhatsNew->setFocusPolicy(Qt::NoFocus);
    txtWhatsNew->setAcceptDrops(false);
    txtWhatsNew->setLineWidth(2);
    txtWhatsNew->setReadOnly(true);

    gridLayout2->addWidget(txtWhatsNew, 0, 0, 1, 1);

    tabWidget->addTab(Widget3, QString());
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    gridLayout3 = new QGridLayout(tab);
    gridLayout3->setSpacing(6);
    gridLayout3->setMargin(9);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    listBox1 = new QListWidget(tab);
    listBox1->setObjectName(QString::fromUtf8("listBox1"));

    gridLayout3->addWidget(listBox1, 1, 0, 1, 1);

    frame = new QFrame(tab);
    frame->setObjectName(QString::fromUtf8("frame"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(5));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    gridLayout4 = new QGridLayout(frame);
    gridLayout4->setSpacing(6);
    gridLayout4->setMargin(9);
    gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
    pixAuthorMug = new QLabel(frame);
    pixAuthorMug->setObjectName(QString::fromUtf8("pixAuthorMug"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(pixAuthorMug->sizePolicy().hasHeightForWidth());
    pixAuthorMug->setSizePolicy(sizePolicy1);
    pixAuthorMug->setFrameShape(QFrame::NoFrame);
    pixAuthorMug->setFrameShadow(QFrame::Plain);
    pixAuthorMug->setPixmap(QPixmap());
    pixAuthorMug->setScaledContents(false);
    pixAuthorMug->setAlignment(Qt::AlignCenter);

    gridLayout4->addWidget(pixAuthorMug, 0, 0, 1, 1);


    gridLayout3->addWidget(frame, 1, 1, 1, 1);

    textLabel1 = new QLabel(tab);
    textLabel1->setObjectName(QString::fromUtf8("textLabel1"));

    gridLayout3->addWidget(textLabel1, 0, 0, 1, 2);

    tabWidget->addTab(tab, QString());
    TabPage = new QWidget();
    TabPage->setObjectName(QString::fromUtf8("TabPage"));
    gridLayout5 = new QGridLayout(TabPage);
    gridLayout5->setSpacing(6);
    gridLayout5->setMargin(9);
    gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
    txtBrowserPlugins = new QTextBrowser(TabPage);
    txtBrowserPlugins->setObjectName(QString::fromUtf8("txtBrowserPlugins"));

    gridLayout5->addWidget(txtBrowserPlugins, 0, 0, 1, 1);

    tabWidget->addTab(TabPage, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    gridLayout6 = new QGridLayout(tab_2);
    gridLayout6->setSpacing(6);
    gridLayout6->setMargin(9);
    gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
    txtSponsors = new QTextEdit(tab_2);
    txtSponsors->setObjectName(QString::fromUtf8("txtSponsors"));
    txtSponsors->setReadOnly(true);

    gridLayout6->addWidget(txtSponsors, 0, 0, 1, 1);

    tabWidget->addTab(tab_2, QString());

    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem = new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    buttonCancel = new QPushButton(QgsAbout);
    buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
    buttonCancel->setAutoDefault(true);

    hboxLayout1->addWidget(buttonCancel);


    gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

    QWidget::setTabOrder(tabWidget, btnQgisUser);
    QWidget::setTabOrder(btnQgisUser, btnQgisHome);
    QWidget::setTabOrder(btnQgisHome, lblUrls);
    QWidget::setTabOrder(lblUrls, buttonCancel);
    QWidget::setTabOrder(buttonCancel, txtBrowserPlugins);
    QWidget::setTabOrder(txtBrowserPlugins, listBox1);

    retranslateUi(QgsAbout);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(QgsAbout);
    } // setupUi

    void retranslateUi(QDialog *QgsAbout)
    {
    QgsAbout->setWindowTitle(QApplication::translate("QgsAbout", "About Quantum GIS", 0, QApplication::UnicodeUTF8));
    TextLabel4->setText(QApplication::translate("QgsAbout", "<h2>Quantum GIS (qgis)</h2>", 0, QApplication::UnicodeUTF8));
    lblVersion->setText(QApplication::translate("QgsAbout", "Version", 0, QApplication::UnicodeUTF8));
    lblUrls->setHtml(QApplication::translate("QgsAbout", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:12pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\">Quantum GIS is licensed under the GNU General Public License</p>\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\">http://www.gnu.org/licenses</p></body></html>", 0, QApplication::UnicodeUTF8));
    btnQgisHome->setText(QApplication::translate("QgsAbout", "QGIS Home Page", 0, QApplication::UnicodeUTF8));
    btnQgisUser->setText(QApplication::translate("QgsAbout", "Subscribe to the QGIS-User mailing list", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(Widget2), QApplication::translate("QgsAbout", "About", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(Widget3), QApplication::translate("QgsAbout", "What's New", 0, QApplication::UnicodeUTF8));
    textLabel1->setText(QApplication::translate("QgsAbout", "<h2>QGIS Developers</h2>", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QgsAbout", "Developers", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(TabPage), QApplication::translate("QgsAbout", "Providers", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QgsAbout", "Sponsors", 0, QApplication::UnicodeUTF8));
    buttonCancel->setText(QApplication::translate("QgsAbout", "Ok", 0, QApplication::UnicodeUTF8));
    buttonCancel->setShortcut(QString());
    Q_UNUSED(QgsAbout);
    } // retranslateUi

};

namespace Ui {
    class QgsAbout: public Ui_QgsAbout {};
} // namespace Ui

#endif // UI_QGSABOUT_H