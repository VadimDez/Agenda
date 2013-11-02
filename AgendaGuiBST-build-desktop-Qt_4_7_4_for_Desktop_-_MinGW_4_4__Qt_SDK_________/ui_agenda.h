/********************************************************************************
** Form generated from reading UI file 'agenda.ui'
**
** Created: Sat 14. Apr 23:18:13 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AGENDA_H
#define UI_AGENDA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Agenda
{
public:
    QVBoxLayout *verticalLayout;
    QCalendarWidget *calendar;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QSpinBox *hours;
    QLabel *label_2;
    QSpinBox *spinBox_2;
    QListWidget *listWidget;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *Agenda)
    {
        if (Agenda->objectName().isEmpty())
            Agenda->setObjectName(QString::fromUtf8("Agenda"));
        Agenda->resize(321, 484);
        verticalLayout = new QVBoxLayout(Agenda);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        calendar = new QCalendarWidget(Agenda);
        calendar->setObjectName(QString::fromUtf8("calendar"));

        verticalLayout->addWidget(calendar);

        label = new QLabel(Agenda);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(Agenda);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        hours = new QSpinBox(Agenda);
        hours->setObjectName(QString::fromUtf8("hours"));
        hours->setMaximum(23);

        horizontalLayout->addWidget(hours);

        label_2 = new QLabel(Agenda);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        spinBox_2 = new QSpinBox(Agenda);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMaximum(59);

        horizontalLayout->addWidget(spinBox_2);


        verticalLayout->addLayout(horizontalLayout);

        listWidget = new QListWidget(Agenda);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        textEdit = new QTextEdit(Agenda);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonEdit = new QPushButton(Agenda);
        buttonEdit->setObjectName(QString::fromUtf8("buttonEdit"));

        horizontalLayout_2->addWidget(buttonEdit);

        pushButton = new QPushButton(Agenda);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(Agenda);

        QMetaObject::connectSlotsByName(Agenda);
    } // setupUi

    void retranslateUi(QWidget *Agenda)
    {
        Agenda->setWindowTitle(QApplication::translate("Agenda", "Agenda", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Agenda", "Eventi:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Agenda", "Ore:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Agenda", "Min", 0, QApplication::UnicodeUTF8));
        buttonEdit->setText(QApplication::translate("Agenda", "ADD", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Agenda", "DELETE", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Agenda: public Ui_Agenda {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AGENDA_H
