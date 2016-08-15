/********************************************************************************
** Form generated from reading UI file 'launcher.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAUNCHER_H
#define UI_LAUNCHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Launcher
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QFrame *frame_2;
    QFrame *frame_6;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QFrame *frame_9;
    QLabel *label_5;
    QFrame *frame_10;
    QFrame *line;
    QFrame *frame_17;
    QFrame *frame_3;
    QLineEdit *lineEdit;
    QLabel *label_4;
    QLabel *label_7;
    QFrame *line_3;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QFrame *line_4;
    QFrame *line_5;
    QFrame *line_6;
    QFrame *frame_4;
    QLabel *label;
    QLabel *label_2;
    QFrame *frame_7;
    QTableWidget *tableWidget;
    QFrame *frame_8;
    QFrame *frame_5;
    QFrame *frame_13;
    QFrame *frame_15;
    QLabel *label_6;
    QLabel *label_20;
    QLabel *label_19;
    QLabel *label_22;
    QLabel *label_23;
    QFrame *line_7;
    QFrame *frame_16;
    QLabel *label_21;
    QTableWidget *tableWidget_2;
    QFrame *frame_14;
    QPushButton *btnPlay;
    QLabel *label_14;
    QLabel *label_15;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QLabel *label_13;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QFrame *frame_11;
    QFrame *frame_12;
    QPushButton *pushButton_4;
    QLabel *label_3;

    void setupUi(QMainWindow *Launcher)
    {
        if (Launcher->objectName().isEmpty())
            Launcher->setObjectName(QStringLiteral("Launcher"));
        Launcher->setWindowModality(Qt::NonModal);
        Launcher->resize(900, 572);
        Launcher->setMinimumSize(QSize(900, 572));
        Launcher->setMaximumSize(QSize(900, 572));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 0));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush1(QColor(134, 0, 2, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        Launcher->setPalette(palette);
        Launcher->setStyleSheet(QLatin1String("#Launcher {\n"
"	background: rgba(0, 0, 0, 0);\n"
"}\n"
"\n"
"QToolTip {\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(3, 3, 3, 255), stop:1 rgba(16, 16, 16, 255));\n"
"\n"
"	color: #fff;\n"
"\n"
"	margin: 5px;\n"
"\n"
"	border-style: none;\n"
"}"));
        centralWidget = new QWidget(Launcher);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 10, 881, 551));
        frame->setStyleSheet(QLatin1String("#frame {\n"
"	border-radius: 3px;\n"
"\n"
"	background: url(:/data/bg.png);\n"
"\n"
"	border: 1px solid rgba(0, 0, 0, 255);\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(1, 1, 879, 91));
        frame_2->setStyleSheet(QLatin1String("#frame_2 {\n"
"	background: url(:/data/bg1.jpg);\n"
"\n"
"	border-top-left-radius: 3px;\n"
"	border-top-right-radius: 3px;\n"
"}"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_6 = new QFrame(frame_2);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setGeometry(QRect(0, 0, 879, 31));
        frame_6->setStyleSheet(QLatin1String("#frame_6 {\n"
"	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(100, 0, 0, 210), stop:1 rgba(200, 0, 0, 210));\n"
"\n"
"	border-top: 1px solid rgba(255, 67, 70, 150);\n"
"	border-bottom: 1px solid rgb(0, 0, 0);\n"
"\n"
"	border-top-left-radius: 3px;\n"
"	border-top-right-radius: 3px;\n"
"}"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        pushButton_2 = new QPushButton(frame_6);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(856, 7, 18, 18));
        pushButton_2->setStyleSheet(QLatin1String("#pushButton_2 {\n"
"	background: url(:/data/close.png);\n"
"	\n"
"	border: 0;\n"
"}\n"
"\n"
"#pushButton_2:hover {\n"
"	background: url(:/data/close_hover.png);\n"
"}"));
        pushButton_3 = new QPushButton(frame_6);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(835, 7, 18, 18));
        QPalette palette1;
        pushButton_3->setPalette(palette1);
        pushButton_3->setStyleSheet(QLatin1String("#pushButton_3 {\n"
"	background: url(:/data/minimize.png);\n"
"	\n"
"	border: 0;\n"
"}\n"
"\n"
"#pushButton_3:hover {\n"
"	background: url(:/data/minimize_hover.png);\n"
"}"));
        frame_9 = new QFrame(frame_6);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setGeometry(QRect(0, 0, 691, 31));
        frame_9->setStyleSheet(QLatin1String("#frame_9 {\n"
"	background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 50), stop:1 rgba(0, 0, 0, 0))\n"
"}"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(frame_9);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 0, 221, 31));
        QFont font;
        font.setFamily(QStringLiteral("Lao UI"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        label_5->setFont(font);
        label_5->setStyleSheet(QLatin1String("color: #fff;\n"
"padding-left: 4px;\n"
"padding-top: -2px;"));
        frame_10 = new QFrame(frame_9);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setGeometry(QRect(5, 5, 20, 18));
        frame_10->setStyleSheet(QStringLiteral("background: url(:/data/icon.png)"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        line = new QFrame(frame_9);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(30, 6, 1, 18));
        line->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 50);\n"
"border: 0;"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        frame_17 = new QFrame(frame_9);
        frame_17->setObjectName(QStringLiteral("frame_17"));
        frame_17->setGeometry(QRect(0, 0, 881, 31));
        frame_17->setFrameShape(QFrame::StyledPanel);
        frame_17->setFrameShadow(QFrame::Raised);
        frame_3 = new QFrame(frame_2);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(0, 30, 881, 61));
        frame_3->setStyleSheet(QLatin1String("#frame_3 {\n"
"	background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0.006, stop:0 rgba(0, 0, 0, 200), stop:0.943182 rgba(0, 0, 0, 0), stop:1 rgba(0, 0, 0, 97));\n"
"}"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        lineEdit = new QLineEdit(frame_3);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(540, 10, 331, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI Light"));
        font1.setPointSize(22);
        lineEdit->setFont(font1);
        lineEdit->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 0);\n"
"border: 0;\n"
"color: #fff;\n"
"\n"
"qproperty-alignment: AlignRight;\n"
"\n"
"padding-top: -6px;\n"
"\n"
"selection-background-color: rgba(0, 0, 0, 100);"));
        lineEdit->setMaxLength(14);
        lineEdit->setClearButtonEnabled(false);
        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 0, 321, 61));
        label_4->setFont(font1);
        label_4->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"padding-top: -5px;"));
        label_7 = new QLabel(frame_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(330, 9, 71, 16));
        QFont font2;
        font2.setFamily(QStringLiteral("Segoe UI"));
        label_7->setFont(font2);
        label_7->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"qproperty-alignment: AlignCenter;"));
        line_3 = new QFrame(frame_3);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(330, 10, 1, 41));
        line_3->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 50);\n"
"border: 0;"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        label_8 = new QLabel(frame_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(330, 20, 71, 31));
        QFont font3;
        font3.setFamily(QStringLiteral("Segoe UI"));
        font3.setPointSize(12);
        label_8->setFont(font3);
        label_8->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"qproperty-alignment: AlignCenter;"));
        label_9 = new QLabel(frame_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(400, 9, 71, 16));
        label_9->setFont(font2);
        label_9->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"qproperty-alignment: AlignCenter;"));
        label_10 = new QLabel(frame_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(400, 20, 71, 31));
        label_10->setFont(font3);
        label_10->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"qproperty-alignment: AlignCenter;"));
        label_11 = new QLabel(frame_3);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(470, 9, 71, 16));
        label_11->setFont(font2);
        label_11->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"qproperty-alignment: AlignCenter;"));
        label_12 = new QLabel(frame_3);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(470, 20, 71, 31));
        label_12->setFont(font3);
        label_12->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"qproperty-alignment: AlignCenter;"));
        line_4 = new QFrame(frame_3);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(400, 10, 1, 41));
        line_4->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 50);\n"
"border: 0;"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(frame_3);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(540, 10, 1, 41));
        line_5->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 50);\n"
"border: 0;"));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        line_6 = new QFrame(frame_3);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setGeometry(QRect(470, 10, 1, 41));
        line_6->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 50);\n"
"border: 0;"));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(0, 90, 881, 437));
        frame_4->setStyleSheet(QLatin1String("#frame_4 {\n"
"	/*background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 125), stop:0.568182 rgba(0, 0, 0, 0), stop:1 rgba(0, 0, 0, 125));*/\n"
"\n"
"background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0.002, stop:0 rgba(0, 0, 0, 150), stop:0.943182 rgba(0, 0, 0, 0), stop:1 rgba(0, 0, 0, 75));\n"
"\n"
"	border-bottom-left-radius: 3px;\n"
"	border-bottom-right-radius: 3px;\n"
"}"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame_4);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 17, 611, 25));
        QFont font4;
        font4.setFamily(QStringLiteral("Lao UI"));
        font4.setPointSize(10);
        label->setFont(font4);
        label->setStyleSheet(QLatin1String("	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(100, 0, 0, 190), stop:1 rgba(200, 0, 0, 190));\n"
"\n"
"	border-top: 1px solid rgba(255, 67, 70, 150);\n"
"	border-bottom: 1px solid rgb(0, 0, 0);\n"
"\n"
"	border-left: 1px solid rgba(0, 0, 0, 75);\n"
"	border-right: 1px solid rgba(0, 0, 0, 75);\n"
"\n"
"	border-top-left-radius: 3px;\n"
"	border-top-right-radius: 3px;\n"
"\n"
"	padding-top: -2px;\n"
"\n"
"	color: #fff;"));
        label_2 = new QLabel(frame_4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(630, 17, 241, 25));
        label_2->setFont(font4);
        label_2->setStyleSheet(QLatin1String("	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(100, 0, 0, 190), stop:1 rgba(200, 0, 0, 190));\n"
"\n"
"	border-top: 1px solid rgba(255, 67, 70, 150);\n"
"	border-bottom: 1px solid rgb(0, 0, 0);\n"
"\n"
"	border-left: 1px solid rgba(0, 0, 0, 75);\n"
"	border-right: 1px solid rgba(0, 0, 0, 75);\n"
"\n"
"	border-top-left-radius: 3px;\n"
"	border-top-right-radius: 3px;\n"
"\n"
"	padding-top: -2px;\n"
"\n"
"	color: #fff;"));
        frame_7 = new QFrame(frame_4);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setGeometry(QRect(10, 41, 611, 386));
        frame_7->setStyleSheet(QLatin1String("background: url(:/data/bg2.jpg);\n"
"background-repeat: no-repeat;"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        tableWidget = new QTableWidget(frame_7);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 611, 385));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush3(QColor(0, 0, 0, 150));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        QBrush brush4(QColor(0, 0, 0, 255));
        brush4.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush3);
        QBrush brush5(QColor(0, 0, 0, 75));
        brush5.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush5);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        QBrush brush7(QColor(0, 0, 0, 255));
        brush7.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        tableWidget->setPalette(palette2);
        tableWidget->setStyleSheet(QLatin1String("QHeaderView::section {\n"
"	border: 0;\n"
"	\n"
"	background-color: rgba(0, 0, 0, 200);\n"
"\n"
"	color: #fff;\n"
"\n"
"	outline: 0;\n"
"\n"
"	padding: 5px;\n"
"}\n"
"\n"
"#tableWidget {\n"
"	color: #fff;\n"
"\n"
"	border: 1px solid rgba(0, 0, 0, 50);\n"
"\n"
"	background: rgba(0, 0, 0, 150);\n"
"	selection-background-color: rgba(0, 0, 0, 75);\n"
"	selection-color: #fff;\n"
"\n"
"	outline: 0;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"	color: #fff !important;\n"
"\n"
"	outline: 0;\n"
"}\n"
"\n"
"QTableWidget::item:focus {\n"
"	background: rgba(0, 0, 0, 75);\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"	background: rgba(75, 0, 0, 255);\n"
"}\n"
"\n"
" QScrollBar::handle:vertical {\n"
"	background: rgb(125, 0, 0);\n"
" }"));
        tableWidget->setAlternatingRowColors(false);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setShowGrid(false);
        tableWidget->setSortingEnabled(true);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(7);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget->horizontalHeader()->setHighlightSections(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        frame_8 = new QFrame(frame_4);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setGeometry(QRect(630, 41, 241, 385));
        frame_8->setStyleSheet(QLatin1String("#frame_8 {\n"
"	background: url(:/data/bg3.jpg)  551px 5550px;\n"
"\n"
"	border: 1px solid rgba(0, 0, 0, 150);\n"
"}"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        frame_5 = new QFrame(frame_8);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setGeometry(QRect(0, 0, 241, 324));
        frame_5->setStyleSheet(QLatin1String("#frame_5 {\n"
"	background: rgba(0, 0, 0, 50);\n"
"}"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_13 = new QFrame(frame_5);
        frame_13->setObjectName(QStringLiteral("frame_13"));
        frame_13->setGeometry(QRect(0, 0, 241, 111));
        frame_13->setStyleSheet(QLatin1String("#frame_13 {\n"
"	border-bottom: 1px solid rgba(0, 0, 0, 150);\n"
"\n"
"	background: url(:/data/unknown.jpg);\n"
"}"));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        frame_15 = new QFrame(frame_13);
        frame_15->setObjectName(QStringLiteral("frame_15"));
        frame_15->setGeometry(QRect(0, 0, 241, 26));
        frame_15->setStyleSheet(QLatin1String("#frame_15 {\n"
"	background: rgba(0, 0, 0, 190);\n"
"\n"
"	border-bottom: 1px solid rgba(0, 0, 0, 200);\n"
"}"));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        label_6 = new QLabel(frame_15);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(21, 1, 171, 31));
        label_6->setFont(font4);
        label_6->setStyleSheet(QLatin1String("padding-left: 4px;\n"
"padding-top: -6px;"));
        label_20 = new QLabel(frame_15);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(4, 5, 20, 17));
        label_20->setStyleSheet(QStringLiteral("background: url(:/data/map.png);"));
        label_19 = new QLabel(frame_15);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(20, 0, 171, 31));
        label_19->setFont(font4);
        label_19->setStyleSheet(QLatin1String("padding-left: 4px;\n"
"padding-top: -6px;\n"
"\n"
"color: #fff;"));
        label_22 = new QLabel(frame_15);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(181, 1, 61, 31));
        label_22->setFont(font4);
        label_22->setStyleSheet(QLatin1String("padding-top: 4px;\n"
"padding-right: 3px;\n"
"\n"
"qproperty-alignment: AlignRight;"));
        label_23 = new QLabel(frame_15);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(180, 0, 61, 31));
        label_23->setFont(font4);
        label_23->setStyleSheet(QLatin1String("padding-top: 4px;\n"
"padding-right: 3px;\n"
"\n"
"qproperty-alignment: AlignRight;\n"
"\n"
"color: #fff;"));
        line_7 = new QFrame(frame_15);
        line_7->setObjectName(QStringLiteral("line_7"));
        line_7->setGeometry(QRect(191, 5, 1, 17));
        line_7->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 75);\n"
"border: 0;"));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);
        frame_16 = new QFrame(frame_13);
        frame_16->setObjectName(QStringLiteral("frame_16"));
        frame_16->setGeometry(QRect(0, 70, 241, 41));
        frame_16->setStyleSheet(QStringLiteral("background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 175), stop:1 rgba(0, 0, 0, 0));"));
        frame_16->setFrameShape(QFrame::StyledPanel);
        frame_16->setFrameShadow(QFrame::Raised);
        label_21 = new QLabel(frame_16);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(100, 20, 135, 21));
        label_21->setStyleSheet(QLatin1String("color: #fff;\n"
"\n"
"background: none;\n"
"outline: none;\n"
"\n"
"qproperty-alignment: AlignRight;"));
        tableWidget_2 = new QTableWidget(frame_5);
        if (tableWidget_2->columnCount() < 2)
            tableWidget_2->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(0, 110, 241, 214));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        QBrush brush8(QColor(0, 0, 0, 255));
        brush8.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Highlight, brush5);
        palette3.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        QBrush brush9(QColor(0, 0, 0, 255));
        brush9.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Highlight, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        QBrush brush10(QColor(0, 0, 0, 255));
        brush10.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Highlight, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        tableWidget_2->setPalette(palette3);
        tableWidget_2->setStyleSheet(QLatin1String("QHeaderView::section {\n"
"	border: 0;\n"
"	\n"
"	background-color: rgba(0, 0, 0, 200);\n"
"\n"
"	color: #fff;\n"
"\n"
"	outline: 0;\n"
"\n"
"	padding: 5px;\n"
"}\n"
"\n"
"#tableWidget_2 {\n"
"	color: #fff;\n"
"\n"
"	border: 1px solid rgba(0, 0, 0, 50);\n"
"\n"
"	background: rgba(0, 0, 0, 150);\n"
"	selection-background-color: rgba(0, 0, 0, 75);\n"
"	selection-color: #fff;\n"
"\n"
"	outline: 0;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"	color: #fff;\n"
"\n"
"	outline: 0;\n"
"}\n"
"\n"
"QTableWidget::item:focus {\n"
"	background: rgba(0, 0, 0, 75);\n"
"}"));
        tableWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_2->setAutoScroll(true);
        tableWidget_2->setAlternatingRowColors(false);
        tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_2->setShowGrid(false);
        tableWidget_2->setSortingEnabled(false);
        tableWidget_2->setCornerButtonEnabled(false);
        tableWidget_2->setRowCount(0);
        tableWidget_2->setColumnCount(2);
        tableWidget_2->horizontalHeader()->setVisible(false);
        tableWidget_2->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget_2->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget_2->horizontalHeader()->setHighlightSections(true);
        tableWidget_2->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget_2->horizontalHeader()->setStretchLastSection(true);
        tableWidget_2->verticalHeader()->setVisible(false);
        frame_14 = new QFrame(frame_8);
        frame_14->setObjectName(QStringLiteral("frame_14"));
        frame_14->setGeometry(QRect(0, 324, 241, 61));
        frame_14->setStyleSheet(QStringLiteral("background: rgba(0, 0, 0, 200);"));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        btnPlay = new QPushButton(frame_14);
        btnPlay->setObjectName(QStringLiteral("btnPlay"));
        btnPlay->setGeometry(QRect(10, 10, 221, 45));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QLinearGradient gradient(0.006, 1, 0, 0);
        gradient.setSpread(QGradient::PadSpread);
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient.setColorAt(0.0113636, QColor(75, 0, 0, 225));
        gradient.setColorAt(1, QColor(175, 0, 0, 225));
        QBrush brush11(gradient);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        QLinearGradient gradient1(0.006, 1, 0, 0);
        gradient1.setSpread(QGradient::PadSpread);
        gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient1.setColorAt(0.0113636, QColor(75, 0, 0, 225));
        gradient1.setColorAt(1, QColor(175, 0, 0, 225));
        QBrush brush12(gradient1);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush12);
        QLinearGradient gradient2(0.006, 1, 0, 0);
        gradient2.setSpread(QGradient::PadSpread);
        gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient2.setColorAt(0.0113636, QColor(75, 0, 0, 225));
        gradient2.setColorAt(1, QColor(175, 0, 0, 225));
        QBrush brush13(gradient2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush13);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        QLinearGradient gradient3(0.006, 1, 0, 0);
        gradient3.setSpread(QGradient::PadSpread);
        gradient3.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient3.setColorAt(0.0113636, QColor(75, 0, 0, 225));
        gradient3.setColorAt(1, QColor(175, 0, 0, 225));
        QBrush brush14(gradient3);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush14);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        QLinearGradient gradient4(0.006, 1, 0, 0);
        gradient4.setSpread(QGradient::PadSpread);
        gradient4.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient4.setColorAt(0.0113636, QColor(75, 0, 0, 225));
        gradient4.setColorAt(1, QColor(175, 0, 0, 225));
        QBrush brush15(gradient4);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush15);
        QLinearGradient gradient5(0.006, 1, 0, 0);
        gradient5.setSpread(QGradient::PadSpread);
        gradient5.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient5.setColorAt(0.0113636, QColor(75, 0, 0, 225));
        gradient5.setColorAt(1, QColor(175, 0, 0, 225));
        QBrush brush16(gradient5);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush16);
        QBrush brush17(QColor(175, 175, 175, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush17);
        QLinearGradient gradient6(0.006, 1, 0, 0);
        gradient6.setSpread(QGradient::PadSpread);
        gradient6.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient6.setColorAt(0.0113636, QColor(43, 43, 43, 255));
        gradient6.setColorAt(0.994318, QColor(79, 79, 79, 255));
        QBrush brush18(gradient6);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush17);
        QLinearGradient gradient7(0.006, 1, 0, 0);
        gradient7.setSpread(QGradient::PadSpread);
        gradient7.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient7.setColorAt(0.0113636, QColor(43, 43, 43, 255));
        gradient7.setColorAt(0.994318, QColor(79, 79, 79, 255));
        QBrush brush19(gradient7);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush19);
        QLinearGradient gradient8(0.006, 1, 0, 0);
        gradient8.setSpread(QGradient::PadSpread);
        gradient8.setCoordinateMode(QGradient::ObjectBoundingMode);
        gradient8.setColorAt(0.0113636, QColor(43, 43, 43, 255));
        gradient8.setColorAt(0.994318, QColor(79, 79, 79, 255));
        QBrush brush20(gradient8);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush20);
        btnPlay->setPalette(palette4);
        QFont font5;
        font5.setFamily(QStringLiteral("Lao UI"));
        font5.setPointSize(12);
        btnPlay->setFont(font5);
        btnPlay->setStyleSheet(QLatin1String("#btnPlay {\n"
"	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(75, 0, 0, 225), stop:1 rgba(175, 0, 0, 225));\n"
"\n"
"	border-top: 1px solid rgba(255, 67, 70, 150);\n"
"	border-bottom: 1px solid rgb(0, 0, 0);\n"
"\n"
"	border-radius: 3px;\n"
"\n"
"	padding-top: -4px;\n"
"\n"
"	color: #fff;\n"
"}\n"
"\n"
"#btnPlay:hover {\n"
"	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(100, 0, 0, 255), stop:1 rgba(200, 0, 0, 225));\n"
"}\n"
"\n"
"#btnPlay:pressed {\n"
"	/*background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(218, 77, 0, 255), stop:1 rgba(255, 138, 0, 255));\n"
"\n"
"	border: 1px solid rgba(170, 0, 0, 150);*/\n"
"\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(81, 0, 0, 255), stop:1 rgba(153, 0, 0, 255));\n"
"}\n"
"\n"
"#btnPlay:disabled {\n"
"	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(43, 43, 43, "
                        "255), stop:0.994318 rgba(79, 79, 79, 255));\n"
"\n"
"	border-top: 1px solid rgb(0, 0, 0);\n"
"\n"
"	color: rgb(175, 175, 175);\n"
"}"));
        label_14 = new QLabel(frame_4);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 16, 611, 25));
        label_14->setStyleSheet(QLatin1String("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 50), stop:1 rgba(0, 0, 0, 0));\n"
"\n"
"border-top-left-radius: 3px;\n"
"border-top-right-radius: 3px;"));
        label_15 = new QLabel(frame_4);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(630, 17, 241, 25));
        label_15->setStyleSheet(QLatin1String("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 0), stop:1 rgba(0, 0, 0, 50));\n"
"\n"
"border-top-left-radius: 3px;\n"
"border-top-right-radius: 3px;"));
        pushButton_5 = new QPushButton(frame_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(583, 20, 16, 18));
        QPalette palette5;
        pushButton_5->setPalette(palette5);
        pushButton_5->setStyleSheet(QLatin1String("#pushButton_5 {\n"
"	background: url(:/data/refresh.png);\n"
"	\n"
"	border: 0;\n"
"}\n"
"\n"
"#pushButton_5:hover {\n"
"	background: url(:/data/refresh_hover.png);\n"
"}"));
        pushButton_6 = new QPushButton(frame_4);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(602, 20, 14, 18));
        QPalette palette6;
        pushButton_6->setPalette(palette6);
        pushButton_6->setStyleSheet(QLatin1String("#pushButton_6 {\n"
"	background: url(:/data/direct.png);\n"
"	\n"
"	border: 0;\n"
"}\n"
"\n"
"#pushButton_6:hover {\n"
"	background: url(:/data/direct_hover.png);\n"
"}"));
        label_13 = new QLabel(frame_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(631, 15, 241, 31));
        label_13->setFont(font4);
        label_13->setStyleSheet(QLatin1String("padding-left: 1px;\n"
"padding-top: -2px;\n"
"\n"
"color: rgb(0, 0, 0);"));
        label_16 = new QLabel(frame_4);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(11, 15, 241, 31));
        label_16->setFont(font4);
        label_16->setStyleSheet(QLatin1String("padding-left: 1px;\n"
"padding-top: -2px;\n"
"\n"
"color: rgb(0, 0, 0);"));
        label_17 = new QLabel(frame_4);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(10, 14, 571, 31));
        label_17->setFont(font4);
        label_17->setStyleSheet(QLatin1String("padding-left: 1px;\n"
"padding-top: -2px;\n"
"\n"
"color: rgb(255, 255, 255);"));
        label_18 = new QLabel(frame_4);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(630, 14, 241, 31));
        label_18->setFont(font4);
        label_18->setStyleSheet(QLatin1String("padding-left: 1px;\n"
"padding-top: -2px;\n"
"\n"
"color: #fff;"));
        frame_11 = new QFrame(frame);
        frame_11->setObjectName(QStringLiteral("frame_11"));
        frame_11->setGeometry(QRect(1, 90, 879, 6));
        frame_11->setStyleSheet(QLatin1String("#frame_11 {\n"
"	background-color: qlineargradient(spread:pad, x1:0.006, y1:1, x2:0, y2:0, stop:0.0113636 rgba(100, 0, 0, 190), stop:1 rgba(200, 0, 0, 190));\n"
"\n"
"	border-top: 1px solid rgba(255, 67, 70, 150);\n"
"	border-bottom: 1px solid rgb(0, 0, 0);\n"
"}"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        frame_12 = new QFrame(frame_11);
        frame_12->setObjectName(QStringLiteral("frame_12"));
        frame_12->setGeometry(QRect(0, 0, 881, 16));
        frame_12->setStyleSheet(QStringLiteral("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 100), stop:0.568182 rgba(0, 0, 0, 0), stop:1 rgba(0, 0, 0, 100))"));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        pushButton_4 = new QPushButton(frame);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(859, 530, 18, 18));
        QPalette palette7;
        pushButton_4->setPalette(palette7);
        pushButton_4->setStyleSheet(QLatin1String("#pushButton_4 {\n"
"	background: url(:/data/community.png);\n"
"	\n"
"	border: 0;\n"
"}\n"
"\n"
"#pushButton_4:hover {\n"
"	background: url(:/data/community_hover.png);\n"
"}"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 527, 881, 23));
        QFont font6;
        font6.setFamily(QStringLiteral("Segoe UI"));
        font6.setBold(false);
        font6.setWeight(50);
        label_3->setFont(font6);
        label_3->setStyleSheet(QLatin1String("background: rgba(0, 0, 0, 185);\n"
"color: #fff;\n"
"border: 1px solid rgba(0, 0, 0, 150);\n"
"\n"
"border-bottom-left-radius: 3px;\n"
"border-bottom-right-radius: 3px;\n"
"\n"
"padding-left: 2px;"));
        frame_2->raise();
        frame_4->raise();
        frame_11->raise();
        label_3->raise();
        pushButton_4->raise();
        Launcher->setCentralWidget(centralWidget);

        retranslateUi(Launcher);

        QMetaObject::connectSlotsByName(Launcher);
    } // setupUi

    void retranslateUi(QMainWindow *Launcher)
    {
        Launcher->setWindowTitle(QApplication::translate("Launcher", "Lost Heaven Multiplayer", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_2->setToolTip(QApplication::translate("Launcher", "Close", 0));
#endif // QT_NO_TOOLTIP
        pushButton_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_3->setToolTip(QApplication::translate("Launcher", "Minimize", 0));
#endif // QT_NO_TOOLTIP
        pushButton_3->setText(QString());
        label_5->setText(QApplication::translate("Launcher", "<html><head/><body><p>Lost Heaven Multiplayer</p></body></html>", 0));
#ifndef QT_NO_TOOLTIP
        lineEdit->setToolTip(QApplication::translate("Launcher", "<html><head/><body><p>Click to edit your in-game nickname.<br/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        lineEdit->setInputMask(QString());
        lineEdit->setText(QApplication::translate("Launcher", "Anonymous", 0));
        label_4->setText(QApplication::translate("Launcher", "<html><head/><body><p>Welcome to Lost Heaven</p></body></html>", 0));
        label_7->setText(QApplication::translate("Launcher", "VERSION", 0));
        label_8->setText(QString());
        label_9->setText(QApplication::translate("Launcher", "SERVERS", 0));
        label_10->setText(QApplication::translate("Launcher", "0", 0));
        label_11->setText(QApplication::translate("Launcher", "PLAYERS", 0));
        label_12->setText(QApplication::translate("Launcher", "0", 0));
        label->setText(QString());
        label_2->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Launcher", "Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Launcher", "Gamemode", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Launcher", "Players", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("Launcher", "Ping", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("Launcher", "Address", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("Launcher", "Website", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("Launcher", "Map", 0));
        label_6->setText(QApplication::translate("Launcher", "Unknown", 0));
        label_20->setText(QString());
        label_19->setText(QApplication::translate("Launcher", "Unknown", 0));
        label_22->setText(QApplication::translate("Launcher", "10ms", 0));
        label_23->setText(QApplication::translate("Launcher", "10ms", 0));
        label_21->setText(QString());
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("Launcher", "Item", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QApplication::translate("Launcher", "Value", 0));
#ifndef QT_NO_TOOLTIP
        btnPlay->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnPlay->setText(QApplication::translate("Launcher", "Join Game", 0));
        label_14->setText(QString());
        label_15->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_5->setToolTip(QApplication::translate("Launcher", "Refresh the server list", 0));
#endif // QT_NO_TOOLTIP
        pushButton_5->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_6->setToolTip(QApplication::translate("Launcher", "Connect to a specific server (IP:PORT)", 0));
#endif // QT_NO_TOOLTIP
        pushButton_6->setText(QString());
        label_13->setText(QApplication::translate("Launcher", "Server Information", 0));
        label_16->setText(QApplication::translate("Launcher", "Servers", 0));
        label_17->setText(QApplication::translate("Launcher", "Servers", 0));
        label_18->setText(QApplication::translate("Launcher", "Server Information", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_4->setToolTip(QApplication::translate("Launcher", "Visit the community forum", 0));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QString());
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Launcher: public Ui_Launcher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAUNCHER_H
