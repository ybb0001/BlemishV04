/********************************************************************************
** Form generated from reading UI file 'BlemishV04.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLEMISHV04_H
#define UI_BLEMISHV04_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlemishV04
{
public:
    QPushButton *pushButton_open_image;
    QLabel *label_show_image;
    QPushButton *pushButton_image_processing;
    QPushButton *pushButton_image_processing_2;
    QTextBrowser *textBrowser;
    QTextEdit *Threshold_A;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *Threshold_B;
    QTextEdit *Threshold_C;
    QTextEdit *Threshold_D;
    QTextEdit *edge_Allow;
    QTextEdit *outer_Allow;
    QTextEdit *inner_Allow;
    QLabel *label_7;
    QLabel *label_8;
    QTextEdit *inner_Spec;
    QTextEdit *edge_Spec;
    QTextEdit *outer_Spec;
    QTextEdit *inner_Limit;
    QTextEdit *edge_Limit;
    QTextEdit *outer_Limit;
    QLabel *label_9;
    QTextEdit *inner_Array;
    QTextEdit *edge_Array;
    QTextEdit *outer_Array;
    QLabel *label_10;
    QPushButton *saveValue;
    QLabel *label_11;
    QLabel *label_12;
    QTextEdit *Size1_D_Spec;
    QLabel *label_13;
    QTextEdit *Size1_B_Spec;
    QTextEdit *Size1_C_Spec;
    QLabel *label_14;
    QGroupBox *groupBox;
    QCheckBox *greenMode;
    QTextEdit *out_Size;
    QTextEdit *in_Size;
    QLabel *label_5;
    QLabel *label_6;
    QTextEdit *SecondThd;
    QLabel *label_22;
    QGroupBox *groupBox_2;
    QTextEdit *Size1_A_Spec;
    QCheckBox *blemishMode;
    QLabel *label_17;
    QTextEdit *Size1_Width;
    QLabel *label_15;
    QTextEdit *Region_A;
    QTextEdit *Region_B;
    QTextEdit *Region_C;
    QTextEdit *Size1_Height;
    QTextEdit *Size2_A_Spec;
    QTextEdit *Size2_Height;
    QTextEdit *Size2_C_Spec;
    QTextEdit *Size2_D_Spec;
    QTextEdit *Size2_B_Spec;
    QLabel *label_16;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QTextEdit *Size2_Width;
    QLabel *label_21;
    QPushButton *saveGray;
    QPushButton *BlemishCheck2;
    QPushButton *pushButton_BlemishCheckNew;
    QTextBrowser *log;
    QPushButton *pushButton_OC;
    QPushButton *pushButton_clear;

    void setupUi(QWidget *BlemishV04)
    {
        if (BlemishV04->objectName().isEmpty())
            BlemishV04->setObjectName(QStringLiteral("BlemishV04"));
        BlemishV04->resize(960, 770);
        BlemishV04->setAutoFillBackground(true);
        pushButton_open_image = new QPushButton(BlemishV04);
        pushButton_open_image->setObjectName(QStringLiteral("pushButton_open_image"));
        pushButton_open_image->setGeometry(QRect(859, 630, 91, 30));
        pushButton_open_image->setCheckable(false);
        label_show_image = new QLabel(BlemishV04);
        label_show_image->setObjectName(QStringLiteral("label_show_image"));
        label_show_image->setGeometry(QRect(10, 10, 800, 600));
        label_show_image->setAlignment(Qt::AlignCenter);
        pushButton_image_processing = new QPushButton(BlemishV04);
        pushButton_image_processing->setObjectName(QStringLiteral("pushButton_image_processing"));
        pushButton_image_processing->setEnabled(false);
        pushButton_image_processing->setGeometry(QRect(740, 630, 100, 30));
        pushButton_image_processing_2 = new QPushButton(BlemishV04);
        pushButton_image_processing_2->setObjectName(QStringLiteral("pushButton_image_processing_2"));
        pushButton_image_processing_2->setEnabled(false);
        pushButton_image_processing_2->setGeometry(QRect(860, 700, 91, 30));
        textBrowser = new QTextBrowser(BlemishV04);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(840, 20, 111, 95));
        textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        Threshold_A = new QTextEdit(BlemishV04);
        Threshold_A->setObjectName(QStringLiteral("Threshold_A"));
        Threshold_A->setGeometry(QRect(90, 645, 41, 28));
        label = new QLabel(BlemishV04);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 645, 75, 28));
        label->setScaledContents(false);
        label_2 = new QLabel(BlemishV04);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 675, 75, 28));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3 = new QLabel(BlemishV04);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 705, 75, 28));
        label_3->setLayoutDirection(Qt::RightToLeft);
        label_4 = new QLabel(BlemishV04);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 735, 75, 28));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        Threshold_B = new QTextEdit(BlemishV04);
        Threshold_B->setObjectName(QStringLiteral("Threshold_B"));
        Threshold_B->setGeometry(QRect(90, 675, 41, 28));
        Threshold_C = new QTextEdit(BlemishV04);
        Threshold_C->setObjectName(QStringLiteral("Threshold_C"));
        Threshold_C->setGeometry(QRect(90, 705, 41, 28));
        Threshold_D = new QTextEdit(BlemishV04);
        Threshold_D->setObjectName(QStringLiteral("Threshold_D"));
        Threshold_D->setGeometry(QRect(90, 735, 41, 28));
        edge_Allow = new QTextEdit(BlemishV04);
        edge_Allow->setObjectName(QStringLiteral("edge_Allow"));
        edge_Allow->setGeometry(QRect(140, 705, 41, 28));
        outer_Allow = new QTextEdit(BlemishV04);
        outer_Allow->setObjectName(QStringLiteral("outer_Allow"));
        outer_Allow->setGeometry(QRect(140, 675, 41, 28));
        inner_Allow = new QTextEdit(BlemishV04);
        inner_Allow->setObjectName(QStringLiteral("inner_Allow"));
        inner_Allow->setGeometry(QRect(140, 645, 41, 28));
        label_7 = new QLabel(BlemishV04);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(140, 615, 41, 28));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(BlemishV04);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(190, 615, 41, 28));
        label_8->setAlignment(Qt::AlignCenter);
        inner_Spec = new QTextEdit(BlemishV04);
        inner_Spec->setObjectName(QStringLiteral("inner_Spec"));
        inner_Spec->setGeometry(QRect(190, 645, 41, 28));
        edge_Spec = new QTextEdit(BlemishV04);
        edge_Spec->setObjectName(QStringLiteral("edge_Spec"));
        edge_Spec->setGeometry(QRect(190, 705, 41, 28));
        outer_Spec = new QTextEdit(BlemishV04);
        outer_Spec->setObjectName(QStringLiteral("outer_Spec"));
        outer_Spec->setGeometry(QRect(190, 675, 41, 28));
        inner_Limit = new QTextEdit(BlemishV04);
        inner_Limit->setObjectName(QStringLiteral("inner_Limit"));
        inner_Limit->setGeometry(QRect(240, 645, 41, 28));
        edge_Limit = new QTextEdit(BlemishV04);
        edge_Limit->setObjectName(QStringLiteral("edge_Limit"));
        edge_Limit->setGeometry(QRect(240, 705, 41, 28));
        outer_Limit = new QTextEdit(BlemishV04);
        outer_Limit->setObjectName(QStringLiteral("outer_Limit"));
        outer_Limit->setGeometry(QRect(240, 675, 41, 28));
        label_9 = new QLabel(BlemishV04);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(240, 615, 41, 28));
        label_9->setAlignment(Qt::AlignCenter);
        inner_Array = new QTextEdit(BlemishV04);
        inner_Array->setObjectName(QStringLiteral("inner_Array"));
        inner_Array->setGeometry(QRect(290, 645, 41, 28));
        edge_Array = new QTextEdit(BlemishV04);
        edge_Array->setObjectName(QStringLiteral("edge_Array"));
        edge_Array->setGeometry(QRect(290, 705, 41, 28));
        outer_Array = new QTextEdit(BlemishV04);
        outer_Array->setObjectName(QStringLiteral("outer_Array"));
        outer_Array->setGeometry(QRect(290, 675, 41, 28));
        label_10 = new QLabel(BlemishV04);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(290, 615, 41, 28));
        label_10->setAlignment(Qt::AlignCenter);
        saveValue = new QPushButton(BlemishV04);
        saveValue->setObjectName(QStringLiteral("saveValue"));
        saveValue->setGeometry(QRect(860, 730, 90, 30));
        saveValue->setCheckable(false);
        label_11 = new QLabel(BlemishV04);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(840, 350, 75, 28));
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_12 = new QLabel(BlemishV04);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(840, 290, 75, 28));
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        Size1_D_Spec = new QTextEdit(BlemishV04);
        Size1_D_Spec->setObjectName(QStringLiteral("Size1_D_Spec"));
        Size1_D_Spec->setGeometry(QRect(890, 350, 61, 28));
        label_13 = new QLabel(BlemishV04);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(840, 320, 75, 28));
        label_13->setLayoutDirection(Qt::RightToLeft);
        Size1_B_Spec = new QTextEdit(BlemishV04);
        Size1_B_Spec->setObjectName(QStringLiteral("Size1_B_Spec"));
        Size1_B_Spec->setGeometry(QRect(890, 290, 61, 28));
        Size1_C_Spec = new QTextEdit(BlemishV04);
        Size1_C_Spec->setObjectName(QStringLiteral("Size1_C_Spec"));
        Size1_C_Spec->setGeometry(QRect(890, 320, 61, 28));
        label_14 = new QLabel(BlemishV04);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(840, 260, 75, 28));
        label_14->setScaledContents(false);
        groupBox = new QGroupBox(BlemishV04);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 620, 451, 151));
        greenMode = new QCheckBox(groupBox);
        greenMode->setObjectName(QStringLiteral("greenMode"));
        greenMode->setGeometry(QRect(340, 128, 118, 20));
        greenMode->setChecked(false);
        out_Size = new QTextEdit(groupBox);
        out_Size->setObjectName(QStringLiteral("out_Size"));
        out_Size->setGeometry(QRect(390, 60, 51, 28));
        in_Size = new QTextEdit(groupBox);
        in_Size->setObjectName(QStringLiteral("in_Size"));
        in_Size->setGeometry(QRect(390, 30, 51, 28));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(340, 30, 61, 28));
        label_5->setLayoutDirection(Qt::RightToLeft);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(340, 60, 71, 28));
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        SecondThd = new QTextEdit(groupBox);
        SecondThd->setObjectName(QStringLiteral("SecondThd"));
        SecondThd->setGeometry(QRect(390, 90, 51, 28));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(340, 90, 61, 28));
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_2 = new QGroupBox(BlemishV04);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(830, 125, 131, 491));
        Size1_A_Spec = new QTextEdit(groupBox_2);
        Size1_A_Spec->setObjectName(QStringLiteral("Size1_A_Spec"));
        Size1_A_Spec->setGeometry(QRect(60, 140, 61, 28));
        blemishMode = new QCheckBox(groupBox_2);
        blemishMode->setObjectName(QStringLiteral("blemishMode"));
        blemishMode->setGeometry(QRect(30, 455, 90, 20));
        blemishMode->setChecked(true);
        label_17 = new QLabel(BlemishV04);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(860, 200, 61, 28));
        label_17->setScaledContents(false);
        label_17->setAlignment(Qt::AlignCenter);
        Size1_Width = new QTextEdit(BlemishV04);
        Size1_Width->setObjectName(QStringLiteral("Size1_Width"));
        Size1_Width->setGeometry(QRect(850, 230, 41, 28));
        label_15 = new QLabel(BlemishV04);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(870, 140, 61, 28));
        label_15->setScaledContents(false);
        Region_A = new QTextEdit(BlemishV04);
        Region_A->setObjectName(QStringLiteral("Region_A"));
        Region_A->setGeometry(QRect(840, 170, 31, 28));
        Region_B = new QTextEdit(BlemishV04);
        Region_B->setObjectName(QStringLiteral("Region_B"));
        Region_B->setGeometry(QRect(880, 170, 31, 28));
        Region_C = new QTextEdit(BlemishV04);
        Region_C->setObjectName(QStringLiteral("Region_C"));
        Region_C->setGeometry(QRect(920, 170, 31, 28));
        Size1_Height = new QTextEdit(BlemishV04);
        Size1_Height->setObjectName(QStringLiteral("Size1_Height"));
        Size1_Height->setGeometry(QRect(910, 230, 41, 28));
        Size2_A_Spec = new QTextEdit(BlemishV04);
        Size2_A_Spec->setObjectName(QStringLiteral("Size2_A_Spec"));
        Size2_A_Spec->setGeometry(QRect(890, 450, 61, 28));
        Size2_Height = new QTextEdit(BlemishV04);
        Size2_Height->setObjectName(QStringLiteral("Size2_Height"));
        Size2_Height->setGeometry(QRect(910, 415, 41, 28));
        Size2_C_Spec = new QTextEdit(BlemishV04);
        Size2_C_Spec->setObjectName(QStringLiteral("Size2_C_Spec"));
        Size2_C_Spec->setGeometry(QRect(890, 510, 61, 28));
        Size2_D_Spec = new QTextEdit(BlemishV04);
        Size2_D_Spec->setObjectName(QStringLiteral("Size2_D_Spec"));
        Size2_D_Spec->setGeometry(QRect(890, 540, 61, 28));
        Size2_B_Spec = new QTextEdit(BlemishV04);
        Size2_B_Spec->setObjectName(QStringLiteral("Size2_B_Spec"));
        Size2_B_Spec->setGeometry(QRect(890, 480, 61, 28));
        label_16 = new QLabel(BlemishV04);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(840, 540, 75, 28));
        label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_18 = new QLabel(BlemishV04);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(840, 480, 75, 28));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_19 = new QLabel(BlemishV04);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(840, 450, 75, 28));
        label_19->setScaledContents(false);
        label_20 = new QLabel(BlemishV04);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(850, 385, 71, 28));
        label_20->setScaledContents(false);
        label_20->setAlignment(Qt::AlignCenter);
        Size2_Width = new QTextEdit(BlemishV04);
        Size2_Width->setObjectName(QStringLiteral("Size2_Width"));
        Size2_Width->setGeometry(QRect(850, 415, 41, 28));
        label_21 = new QLabel(BlemishV04);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(840, 510, 75, 28));
        label_21->setLayoutDirection(Qt::RightToLeft);
        saveGray = new QPushButton(BlemishV04);
        saveGray->setObjectName(QStringLiteral("saveGray"));
        saveGray->setEnabled(false);
        saveGray->setGeometry(QRect(860, 670, 90, 30));
        saveGray->setCheckable(false);
        BlemishCheck2 = new QPushButton(BlemishV04);
        BlemishCheck2->setObjectName(QStringLiteral("BlemishCheck2"));
        BlemishCheck2->setEnabled(false);
        BlemishCheck2->setGeometry(QRect(740, 660, 100, 30));
        pushButton_BlemishCheckNew = new QPushButton(BlemishV04);
        pushButton_BlemishCheckNew->setObjectName(QStringLiteral("pushButton_BlemishCheckNew"));
        pushButton_BlemishCheckNew->setEnabled(false);
        pushButton_BlemishCheckNew->setGeometry(QRect(740, 700, 100, 30));
        log = new QTextBrowser(BlemishV04);
        log->setObjectName(QStringLiteral("log"));
        log->setGeometry(QRect(470, 620, 261, 141));
        pushButton_OC = new QPushButton(BlemishV04);
        pushButton_OC->setObjectName(QStringLiteral("pushButton_OC"));
        pushButton_OC->setEnabled(false);
        pushButton_OC->setGeometry(QRect(740, 730, 100, 30));
        pushButton_clear = new QPushButton(BlemishV04);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));
        pushButton_clear->setEnabled(true);
        pushButton_clear->setGeometry(QRect(670, 730, 61, 30));
        groupBox->raise();
        groupBox_2->raise();
        pushButton_open_image->raise();
        pushButton_image_processing->raise();
        pushButton_image_processing_2->raise();
        textBrowser->raise();
        Threshold_A->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        Threshold_B->raise();
        Threshold_C->raise();
        Threshold_D->raise();
        edge_Allow->raise();
        outer_Allow->raise();
        inner_Allow->raise();
        label_7->raise();
        label_8->raise();
        inner_Spec->raise();
        edge_Spec->raise();
        outer_Spec->raise();
        inner_Limit->raise();
        edge_Limit->raise();
        outer_Limit->raise();
        label_9->raise();
        inner_Array->raise();
        edge_Array->raise();
        outer_Array->raise();
        label_10->raise();
        saveValue->raise();
        label_11->raise();
        label_12->raise();
        Size1_D_Spec->raise();
        label_13->raise();
        Size1_B_Spec->raise();
        Size1_C_Spec->raise();
        label_14->raise();
        label_17->raise();
        Size1_Width->raise();
        label_15->raise();
        Region_A->raise();
        Region_B->raise();
        Region_C->raise();
        Size1_Height->raise();
        Size2_A_Spec->raise();
        Size2_Height->raise();
        Size2_C_Spec->raise();
        Size2_D_Spec->raise();
        Size2_B_Spec->raise();
        label_16->raise();
        label_18->raise();
        label_19->raise();
        label_20->raise();
        Size2_Width->raise();
        label_21->raise();
        label_show_image->raise();
        saveGray->raise();
        BlemishCheck2->raise();
        pushButton_BlemishCheckNew->raise();
        log->raise();
        pushButton_OC->raise();
        pushButton_clear->raise();

        retranslateUi(BlemishV04);

        QMetaObject::connectSlotsByName(BlemishV04);
    } // setupUi

    void retranslateUi(QWidget *BlemishV04)
    {
        BlemishV04->setWindowTitle(QApplication::translate("BlemishV04", "BlemishV05", Q_NULLPTR));
        pushButton_open_image->setText(QApplication::translate("BlemishV04", "\346\211\223\345\274\200\345\233\276\345\203\217", Q_NULLPTR));
        label_show_image->setText(QApplication::translate("BlemishV04", "ShowImage", Q_NULLPTR));
        pushButton_image_processing->setText(QApplication::translate("BlemishV04", "\345\274\202\347\211\251\346\265\213\350\257\225Size1", Q_NULLPTR));
        pushButton_image_processing_2->setText(QApplication::translate("BlemishV04", "\350\211\262\347\202\271\346\265\213\350\257\225", Q_NULLPTR));
        textBrowser->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:48pt;\">NA</span></p></body></html>", Q_NULLPTR));
        Threshold_A->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.19</p></body></html>", Q_NULLPTR));
        label->setText(QApplication::translate("BlemishV04", "Inner_A", Q_NULLPTR));
        label_2->setText(QApplication::translate("BlemishV04", "Outer_B", Q_NULLPTR));
        label_3->setText(QApplication::translate("BlemishV04", "Edge_C", Q_NULLPTR));
        label_4->setText(QApplication::translate("BlemishV04", "Corner_D", Q_NULLPTR));
        Threshold_B->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.19</p></body></html>", Q_NULLPTR));
        Threshold_C->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.19</p></body></html>", Q_NULLPTR));
        Threshold_D->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.25</p></body></html>", Q_NULLPTR));
        edge_Allow->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">4</p></body></html>", Q_NULLPTR));
        outer_Allow->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">4</p></body></html>", Q_NULLPTR));
        inner_Allow->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">2</p></body></html>", Q_NULLPTR));
        label_7->setText(QApplication::translate("BlemishV04", "Allow", Q_NULLPTR));
        label_8->setText(QApplication::translate("BlemishV04", "Spec", Q_NULLPTR));
        inner_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">5</p></body></html>", Q_NULLPTR));
        edge_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">5</p></body></html>", Q_NULLPTR));
        outer_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">5</p></body></html>", Q_NULLPTR));
        inner_Limit->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1</p></body></html>", Q_NULLPTR));
        edge_Limit->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1</p></body></html>", Q_NULLPTR));
        outer_Limit->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1</p></body></html>", Q_NULLPTR));
        label_9->setText(QApplication::translate("BlemishV04", "Limit", Q_NULLPTR));
        inner_Array->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">3</p></body></html>", Q_NULLPTR));
        edge_Array->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">5</p></body></html>", Q_NULLPTR));
        outer_Array->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">5</p></body></html>", Q_NULLPTR));
        label_10->setText(QApplication::translate("BlemishV04", "Array", Q_NULLPTR));
        saveValue->setText(QApplication::translate("BlemishV04", "Save Spec", Q_NULLPTR));
        label_11->setText(QApplication::translate("BlemishV04", "Corner", Q_NULLPTR));
        label_12->setText(QApplication::translate("BlemishV04", "Outer", Q_NULLPTR));
        Size1_D_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.15</p></body></html>", Q_NULLPTR));
        label_13->setText(QApplication::translate("BlemishV04", "Edge", Q_NULLPTR));
        Size1_B_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.006</p></body></html>", Q_NULLPTR));
        Size1_C_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.006</p></body></html>", Q_NULLPTR));
        label_14->setText(QApplication::translate("BlemishV04", "Inner", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        groupBox->setToolTip(QApplication::translate("BlemishV04", "<html><head/><body><p>11111</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        groupBox->setTitle(QApplication::translate("BlemishV04", "W/B DOT TEST SET", Q_NULLPTR));
        greenMode->setText(QApplication::translate("BlemishV04", "Green Mode", Q_NULLPTR));
        out_Size->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.333</p></body></html>", Q_NULLPTR));
        in_Size->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.333</p></body></html>", Q_NULLPTR));
        label_5->setText(QApplication::translate("BlemishV04", "In_Size", Q_NULLPTR));
        label_6->setText(QApplication::translate("BlemishV04", "Out_Size", Q_NULLPTR));
        SecondThd->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1.000</p></body></html>", Q_NULLPTR));
        label_22->setText(QApplication::translate("BlemishV04", "2nd Thd", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("BlemishV04", "Blemish TEST", Q_NULLPTR));
        Size1_A_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.005</p></body></html>", Q_NULLPTR));
        blemishMode->setText(QApplication::translate("BlemishV04", "AVG Mode", Q_NULLPTR));
        label_17->setText(QApplication::translate("BlemishV04", "<html><head/><body><p><span style=\" font-size:10pt; font-weight:600;\">Size_1</span></p></body></html>", Q_NULLPTR));
        Size1_Width->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">48</p></body></html>", Q_NULLPTR));
        label_15->setText(QApplication::translate("BlemishV04", "<html><head/><body><p><span style=\" font-size:10pt; font-weight:600;\">Region</span></p></body></html>", Q_NULLPTR));
        Region_A->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">60</p></body></html>", Q_NULLPTR));
        Region_B->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">80</p></body></html>", Q_NULLPTR));
        Region_C->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">100</p></body></html>", Q_NULLPTR));
        Size1_Height->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">36</p></body></html>", Q_NULLPTR));
        Size2_A_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1.000</p></body></html>", Q_NULLPTR));
        Size2_Height->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">21</p></body></html>", Q_NULLPTR));
        Size2_C_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.009</p></body></html>", Q_NULLPTR));
        Size2_D_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.150</p></body></html>", Q_NULLPTR));
        Size2_B_Spec->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">1.000</p></body></html>", Q_NULLPTR));
        label_16->setText(QApplication::translate("BlemishV04", "Corner", Q_NULLPTR));
        label_18->setText(QApplication::translate("BlemishV04", "Outer", Q_NULLPTR));
        label_19->setText(QApplication::translate("BlemishV04", "Inner", Q_NULLPTR));
        label_20->setText(QApplication::translate("BlemishV04", "<html><head/><body><p><span style=\" font-size:10pt; font-weight:600;\">Size_2</span></p></body></html>", Q_NULLPTR));
        Size2_Width->setHtml(QApplication::translate("BlemishV04", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"right\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">24</p></body></html>", Q_NULLPTR));
        label_21->setText(QApplication::translate("BlemishV04", "Edge", Q_NULLPTR));
        saveGray->setText(QApplication::translate("BlemishV04", "Save Gray", Q_NULLPTR));
        BlemishCheck2->setText(QApplication::translate("BlemishV04", "\345\274\202\347\211\251\346\265\213\350\257\225Size2", Q_NULLPTR));
        pushButton_BlemishCheckNew->setText(QApplication::translate("BlemishV04", "\345\274\202\347\211\251\346\226\260\347\256\227\346\263\225", Q_NULLPTR));
        pushButton_OC->setText(QApplication::translate("BlemishV04", "OC Test", Q_NULLPTR));
        pushButton_clear->setText(QApplication::translate("BlemishV04", "Clear", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BlemishV04: public Ui_BlemishV04 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLEMISHV04_H
