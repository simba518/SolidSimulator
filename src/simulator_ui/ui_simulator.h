/********************************************************************************
** Form generated from reading UI file 'simulator.ui'
**
** Created: Thu Apr 24 18:16:43 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMULATOR_H
#define UI_SIMULATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "QGLViewerExt.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoadInitFile;
    QAction *actionExit;
    QAction *actionLoadObj;
    QAction *actionLoadVol;
    QAction *actionVolMesh;
    QAction *actionObjMesh;
    QAction *actionConNodes;
    QAction *actionPrepareSimulation;
    QAction *actionPauseSimulation;
    QAction *actionReset;
    QAction *actionPrintSelected;
    QAction *actionRecordDisp;
    QAction *actionClearRecordDisp;
    QAction *actionSaveRecordDisp;
    QAction *actionSaveMtlAsVTK;
    QAction *actionSaveRecordVTK;
    QAction *actionRestVolMesh;
    QAction *actionGround;
    QAction *actionSaveVol;
    QAction *actionRestObjMesh;
    QAction *actionSaveConNodes;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QGLViewerExt *left_view;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menuView;
    QMenu *menuSimulation;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(828, 666);
        actionLoadInitFile = new QAction(MainWindow);
        actionLoadInitFile->setObjectName(QString::fromUtf8("actionLoadInitFile"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionLoadObj = new QAction(MainWindow);
        actionLoadObj->setObjectName(QString::fromUtf8("actionLoadObj"));
        actionLoadVol = new QAction(MainWindow);
        actionLoadVol->setObjectName(QString::fromUtf8("actionLoadVol"));
        actionVolMesh = new QAction(MainWindow);
        actionVolMesh->setObjectName(QString::fromUtf8("actionVolMesh"));
        actionVolMesh->setCheckable(true);
        actionVolMesh->setChecked(true);
        actionObjMesh = new QAction(MainWindow);
        actionObjMesh->setObjectName(QString::fromUtf8("actionObjMesh"));
        actionObjMesh->setCheckable(true);
        actionObjMesh->setChecked(true);
        actionConNodes = new QAction(MainWindow);
        actionConNodes->setObjectName(QString::fromUtf8("actionConNodes"));
        actionConNodes->setCheckable(true);
        actionConNodes->setChecked(true);
        actionPrepareSimulation = new QAction(MainWindow);
        actionPrepareSimulation->setObjectName(QString::fromUtf8("actionPrepareSimulation"));
        actionPauseSimulation = new QAction(MainWindow);
        actionPauseSimulation->setObjectName(QString::fromUtf8("actionPauseSimulation"));
        actionPauseSimulation->setCheckable(true);
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        actionPrintSelected = new QAction(MainWindow);
        actionPrintSelected->setObjectName(QString::fromUtf8("actionPrintSelected"));
        actionPrintSelected->setCheckable(true);
        actionRecordDisp = new QAction(MainWindow);
        actionRecordDisp->setObjectName(QString::fromUtf8("actionRecordDisp"));
        actionRecordDisp->setCheckable(true);
        actionClearRecordDisp = new QAction(MainWindow);
        actionClearRecordDisp->setObjectName(QString::fromUtf8("actionClearRecordDisp"));
        actionSaveRecordDisp = new QAction(MainWindow);
        actionSaveRecordDisp->setObjectName(QString::fromUtf8("actionSaveRecordDisp"));
        actionSaveMtlAsVTK = new QAction(MainWindow);
        actionSaveMtlAsVTK->setObjectName(QString::fromUtf8("actionSaveMtlAsVTK"));
        actionSaveRecordVTK = new QAction(MainWindow);
        actionSaveRecordVTK->setObjectName(QString::fromUtf8("actionSaveRecordVTK"));
        actionRestVolMesh = new QAction(MainWindow);
        actionRestVolMesh->setObjectName(QString::fromUtf8("actionRestVolMesh"));
        actionRestVolMesh->setCheckable(true);
        actionGround = new QAction(MainWindow);
        actionGround->setObjectName(QString::fromUtf8("actionGround"));
        actionGround->setCheckable(true);
        actionSaveVol = new QAction(MainWindow);
        actionSaveVol->setObjectName(QString::fromUtf8("actionSaveVol"));
        actionRestObjMesh = new QAction(MainWindow);
        actionRestObjMesh->setObjectName(QString::fromUtf8("actionRestObjMesh"));
        actionRestObjMesh->setCheckable(true);
        actionSaveConNodes = new QAction(MainWindow);
        actionSaveConNodes->setObjectName(QString::fromUtf8("actionSaveConNodes"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setLineWidth(12);
        splitter->setMidLineWidth(10);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(12);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        left_view = new QGLViewerExt(widget);
        left_view->setObjectName(QString::fromUtf8("left_view"));

        verticalLayout->addWidget(left_view);

        splitter->addWidget(widget);
        splitter_2->addWidget(splitter);

        verticalLayout_2->addWidget(splitter_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 828, 25));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuSimulation = new QMenu(menubar);
        menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuSimulation->menuAction());
        menu_File->addAction(actionLoadInitFile);
        menu_File->addAction(actionLoadObj);
        menu_File->addAction(actionLoadVol);
        menu_File->addAction(actionSaveVol);
        menu_File->addAction(actionSaveRecordDisp);
        menu_File->addAction(actionSaveMtlAsVTK);
        menu_File->addAction(actionSaveRecordVTK);
        menu_File->addAction(actionSaveConNodes);
        menu_File->addAction(actionPrintSelected);
        menu_File->addAction(actionExit);
        menuView->addAction(actionVolMesh);
        menuView->addAction(actionObjMesh);
        menuView->addAction(actionConNodes);
        menuView->addAction(actionRestVolMesh);
        menuView->addAction(actionRestObjMesh);
        menuView->addAction(actionGround);
        menuSimulation->addAction(actionPrepareSimulation);
        menuSimulation->addAction(actionPauseSimulation);
        menuSimulation->addAction(actionReset);
        menuSimulation->addAction(actionRecordDisp);
        menuSimulation->addAction(actionClearRecordDisp);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PhyAE", 0, QApplication::UnicodeUTF8));
        actionLoadInitFile->setText(QApplication::translate("MainWindow", "LoadInitFile", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Q", 0, QApplication::UnicodeUTF8));
        actionLoadObj->setText(QApplication::translate("MainWindow", "LoadObj", 0, QApplication::UnicodeUTF8));
        actionLoadVol->setText(QApplication::translate("MainWindow", "LoadVol", 0, QApplication::UnicodeUTF8));
        actionVolMesh->setText(QApplication::translate("MainWindow", "VolMesh", 0, QApplication::UnicodeUTF8));
        actionVolMesh->setShortcut(QApplication::translate("MainWindow", "V", 0, QApplication::UnicodeUTF8));
        actionObjMesh->setText(QApplication::translate("MainWindow", "ObjMesh", 0, QApplication::UnicodeUTF8));
        actionObjMesh->setShortcut(QApplication::translate("MainWindow", "W", 0, QApplication::UnicodeUTF8));
        actionConNodes->setText(QApplication::translate("MainWindow", "ConNodes", 0, QApplication::UnicodeUTF8));
        actionPrepareSimulation->setText(QApplication::translate("MainWindow", "PrepareSimulation", 0, QApplication::UnicodeUTF8));
        actionPrepareSimulation->setShortcut(QApplication::translate("MainWindow", "E", 0, QApplication::UnicodeUTF8));
        actionPauseSimulation->setText(QApplication::translate("MainWindow", "PauseSimulation", 0, QApplication::UnicodeUTF8));
        actionPauseSimulation->setShortcut(QApplication::translate("MainWindow", "Space", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        actionReset->setShortcut(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        actionPrintSelected->setText(QApplication::translate("MainWindow", "PrintSelected", 0, QApplication::UnicodeUTF8));
        actionRecordDisp->setText(QApplication::translate("MainWindow", "RecordDisp", 0, QApplication::UnicodeUTF8));
        actionRecordDisp->setShortcut(QApplication::translate("MainWindow", "C", 0, QApplication::UnicodeUTF8));
        actionClearRecordDisp->setText(QApplication::translate("MainWindow", "ClearRecordDisp", 0, QApplication::UnicodeUTF8));
        actionSaveRecordDisp->setText(QApplication::translate("MainWindow", "SaveRecordDisp", 0, QApplication::UnicodeUTF8));
        actionSaveMtlAsVTK->setText(QApplication::translate("MainWindow", "SaveMtlAsVTK", 0, QApplication::UnicodeUTF8));
        actionSaveRecordVTK->setText(QApplication::translate("MainWindow", "SaveRecordVTK", 0, QApplication::UnicodeUTF8));
        actionRestVolMesh->setText(QApplication::translate("MainWindow", "RestVolMesh", 0, QApplication::UnicodeUTF8));
        actionGround->setText(QApplication::translate("MainWindow", "Ground", 0, QApplication::UnicodeUTF8));
        actionSaveVol->setText(QApplication::translate("MainWindow", "SaveVol", 0, QApplication::UnicodeUTF8));
        actionRestObjMesh->setText(QApplication::translate("MainWindow", "RestObjMesh", 0, QApplication::UnicodeUTF8));
        actionRestObjMesh->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionSaveConNodes->setText(QApplication::translate("MainWindow", "SaveConNodes", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuSimulation->setTitle(QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMULATOR_H
