/********************************************************************************
** Form generated from reading ui file 'dissolve.ui'
**
** Created: Tue Jul 27 23:38:17 2010
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DISSOLVE_H
#define UI_DISSOLVE_H

#include <Qt3Support/Q3TextBrowser>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

class Ui_Dissolve
{
public:
    QGroupBox *groupBox;
    QPushButton *pushButtonInput;
    QLineEdit *lineEditPolygonInput;
    QLabel *label;
    QGroupBox *groupBox_2;
    QPushButton *pushButtonOutput;
    QLabel *label_2;
    QLineEdit *lineEditPolygonOutput;
    QGroupBox *groupBox_3;
    QPushButton *pushButtonHelp;
    QPushButton *pushButtonClose;
    QPushButton *pushButtonRun;
    Q3TextBrowser *textBrowser;

    void setupUi(QDialog *Dissolve)
    {
    if (Dissolve->objectName().isEmpty())
        Dissolve->setObjectName(QString::fromUtf8("Dissolve"));
    Dissolve->resize(635, 361);
    groupBox = new QGroupBox(Dissolve);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(18, 3, 600, 71));
    pushButtonInput = new QPushButton(groupBox);
    pushButtonInput->setObjectName(QString::fromUtf8("pushButtonInput"));
    pushButtonInput->setGeometry(QRect(490, 30, 98, 32));
    lineEditPolygonInput = new QLineEdit(groupBox);
    lineEditPolygonInput->setObjectName(QString::fromUtf8("lineEditPolygonInput"));
    lineEditPolygonInput->setGeometry(QRect(185, 34, 281, 22));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 34, 161, 18));
    groupBox_2 = new QGroupBox(Dissolve);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(18, 78, 600, 71));
    pushButtonOutput = new QPushButton(groupBox_2);
    pushButtonOutput->setObjectName(QString::fromUtf8("pushButtonOutput"));
    pushButtonOutput->setGeometry(QRect(490, 30, 98, 32));
    label_2 = new QLabel(groupBox_2);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(19, 37, 161, 18));
    lineEditPolygonOutput = new QLineEdit(groupBox_2);
    lineEditPolygonOutput->setObjectName(QString::fromUtf8("lineEditPolygonOutput"));
    lineEditPolygonOutput->setGeometry(QRect(185, 34, 281, 22));
    groupBox_3 = new QGroupBox(Dissolve);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setGeometry(QRect(18, 168, 600, 47));
    pushButtonHelp = new QPushButton(groupBox_3);
    pushButtonHelp->setObjectName(QString::fromUtf8("pushButtonHelp"));
    pushButtonHelp->setGeometry(QRect(10, 9, 98, 32));
    pushButtonClose = new QPushButton(groupBox_3);
    pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
    pushButtonClose->setGeometry(QRect(350, 9, 98, 32));
    pushButtonRun = new QPushButton(groupBox_3);
    pushButtonRun->setObjectName(QString::fromUtf8("pushButtonRun"));
    pushButtonRun->setGeometry(QRect(490, 9, 98, 32));
    textBrowser = new Q3TextBrowser(Dissolve);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    textBrowser->setGeometry(QRect(18, 227, 600, 121));

    retranslateUi(Dissolve);

    QMetaObject::connectSlotsByName(Dissolve);
    } // setupUi

    void retranslateUi(QDialog *Dissolve)
    {
    Dissolve->setWindowTitle(QApplication::translate("Dissolve", "Dissolve", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("Dissolve", "Input", 0, QApplication::UnicodeUTF8));
    pushButtonInput->setToolTip(QApplication::translate("Dissolve", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Browse the polygon file you want to dissove!</p></body></html>", 0, QApplication::UnicodeUTF8));
    pushButtonInput->setText(QApplication::translate("Dissolve", "Browse", 0, QApplication::UnicodeUTF8));
    label->setToolTip(QApplication::translate("Dissolve", "A MULTI-polygon shapefile (multiple watershed boundary)\n"
"that needs to be dissolved to one polygon shapefile.", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dissolve", "Input Polygon File", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("Dissolve", "Output", 0, QApplication::UnicodeUTF8));
    pushButtonOutput->setToolTip(QApplication::translate("Dissolve", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Browse a file name to which dissolve file will be saved!</p></body></html>", 0, QApplication::UnicodeUTF8));
    pushButtonOutput->setText(QApplication::translate("Dissolve", "Browse", 0, QApplication::UnicodeUTF8));
    label_2->setToolTip(QApplication::translate("Dissolve", "Dissolved polygon shapefile", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Dissolve", "Output Dissolve File", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QString());
    pushButtonHelp->setText(QApplication::translate("Dissolve", "Help", 0, QApplication::UnicodeUTF8));
    pushButtonClose->setText(QApplication::translate("Dissolve", "Close", 0, QApplication::UnicodeUTF8));
    pushButtonRun->setText(QApplication::translate("Dissolve", "Run!", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Dissolve);
    } // retranslateUi

};

namespace Ui {
    class Dissolve: public Ui_Dissolve {};
} // namespace Ui

#endif // UI_DISSOLVE_H