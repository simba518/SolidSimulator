/********************************************************************************
** Form generated from reading UI file 'simulator.ui'
**
** Created: Thu Apr 3 19:06:18 2014
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
    QAction *actionSimulate;
    QAction *actionLoadObj;
    QAction *actionLoadVol;
    QAction *actionSaveConNodes;
    QAction *actionLoadConNodes;
    QAction *actionSelectNodes;
    QAction *actionSelectTets;
    QAction *actionVolMesh;
    QAction *actionObjMesh;
    QAction *actionTetGroups;
    QAction *actionConNodes;
    QAction *actionMaterialGroups;
    QAction *actionPrepareSimulation;
    QAction *actionPauseSimulation;
    QAction *actionStopSimulation;
    QAction *actionReset;
    QAction *actionFixSelectedTets;
    QAction *actionSaveMaterial;
    QAction *actionPrintSelected;
    QAction *actionLoadMaterial;
    QAction *actionRecord;
    QAction *actionSaveRecordZ;
    QAction *actionSaveEigenValues;
    QAction *actionSaveEigenVectors;
    QAction *actionClearRecordedZ;
    QAction *actionSaveMaterialAsHinp;
    QAction *actionInterpolateMaterials;
    QAction *actionRecordDisp;
    QAction *actionClearRecordDisp;
    QAction *actionSaveRecordDisp;
    QAction *actionSaveMtlAsVTK;
    QAction *actionSaveRecordVTK;
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
        actionSimulate = new QAction(MainWindow);
        actionSimulate->setObjectName(QString::fromUtf8("actionSimulate"));
        actionLoadObj = new QAction(MainWindow);
        actionLoadObj->setObjectName(QString::fromUtf8("actionLoadObj"));
        actionLoadVol = new QAction(MainWindow);
        actionLoadVol->setObjectName(QString::fromUtf8("actionLoadVol"));
        actionSaveConNodes = new QAction(MainWindow);
        actionSaveConNodes->setObjectName(QString::fromUtf8("actionSaveConNodes"));
        actionLoadConNodes = new QAction(MainWindow);
        actionLoadConNodes->setObjectName(QString::fromUtf8("actionLoadConNodes"));
        actionSelectNodes = new QAction(MainWindow);
        actionSelectNodes->setObjectName(QString::fromUtf8("actionSelectNodes"));
        actionSelectTets = new QAction(MainWindow);
        actionSelectTets->setObjectName(QString::fromUtf8("actionSelectTets"));
        actionVolMesh = new QAction(MainWindow);
        actionVolMesh->setObjectName(QString::fromUtf8("actionVolMesh"));
        actionVolMesh->setCheckable(true);
        actionVolMesh->setChecked(true);
        actionObjMesh = new QAction(MainWindow);
        actionObjMesh->setObjectName(QString::fromUtf8("actionObjMesh"));
        actionObjMesh->setCheckable(true);
        actionObjMesh->setChecked(true);
        actionTetGroups = new QAction(MainWindow);
        actionTetGroups->setObjectName(QString::fromUtf8("actionTetGroups"));
        actionConNodes = new QAction(MainWindow);
        actionConNodes->setObjectName(QString::fromUtf8("actionConNodes"));
        actionConNodes->setCheckable(true);
        actionConNodes->setChecked(true);
        actionMaterialGroups = new QAction(MainWindow);
        actionMaterialGroups->setObjectName(QString::fromUtf8("actionMaterialGroups"));
        actionPrepareSimulation = new QAction(MainWindow);
        actionPrepareSimulation->setObjectName(QString::fromUtf8("actionPrepareSimulation"));
        actionPauseSimulation = new QAction(MainWindow);
        actionPauseSimulation->setObjectName(QString::fromUtf8("actionPauseSimulation"));
        actionPauseSimulation->setCheckable(true);
        actionStopSimulation = new QAction(MainWindow);
        actionStopSimulation->setObjectName(QString::fromUtf8("actionStopSimulation"));
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        actionFixSelectedTets = new QAction(MainWindow);
        actionFixSelectedTets->setObjectName(QString::fromUtf8("actionFixSelectedTets"));
        actionFixSelectedTets->setCheckable(true);
        actionSaveMaterial = new QAction(MainWindow);
        actionSaveMaterial->setObjectName(QString::fromUtf8("actionSaveMaterial"));
        actionPrintSelected = new QAction(MainWindow);
        actionPrintSelected->setObjectName(QString::fromUtf8("actionPrintSelected"));
        actionPrintSelected->setCheckable(true);
        actionLoadMaterial = new QAction(MainWindow);
        actionLoadMaterial->setObjectName(QString::fromUtf8("actionLoadMaterial"));
        actionRecord = new QAction(MainWindow);
        actionRecord->setObjectName(QString::fromUtf8("actionRecord"));
        actionRecord->setCheckable(true);
        actionSaveRecordZ = new QAction(MainWindow);
        actionSaveRecordZ->setObjectName(QString::fromUtf8("actionSaveRecordZ"));
        actionSaveEigenValues = new QAction(MainWindow);
        actionSaveEigenValues->setObjectName(QString::fromUtf8("actionSaveEigenValues"));
        actionSaveEigenVectors = new QAction(MainWindow);
        actionSaveEigenVectors->setObjectName(QString::fromUtf8("actionSaveEigenVectors"));
        actionClearRecordedZ = new QAction(MainWindow);
        actionClearRecordedZ->setObjectName(QString::fromUtf8("actionClearRecordedZ"));
        actionSaveMaterialAsHinp = new QAction(MainWindow);
        actionSaveMaterialAsHinp->setObjectName(QString::fromUtf8("actionSaveMaterialAsHinp"));
        actionInterpolateMaterials = new QAction(MainWindow);
        actionInterpolateMaterials->setObjectName(QString::fromUtf8("actionInterpolateMaterials"));
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
        menu_File->addAction(actionSaveRecordDisp);
        menu_File->addAction(actionSaveMtlAsVTK);
        menu_File->addAction(actionSaveRecordVTK);
        menu_File->addAction(actionPrintSelected);
        menu_File->addAction(actionExit);
        menuView->addAction(actionVolMesh);
        menuView->addAction(actionObjMesh);
        menuView->addAction(actionConNodes);
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
        actionSimulate->setText(QApplication::translate("MainWindow", "Simulate", 0, QApplication::UnicodeUTF8));
        actionLoadObj->setText(QApplication::translate("MainWindow", "LoadObj", 0, QApplication::UnicodeUTF8));
        actionLoadVol->setText(QApplication::translate("MainWindow", "LoadVol", 0, QApplication::UnicodeUTF8));
        actionSaveConNodes->setText(QApplication::translate("MainWindow", "SaveConNodes", 0, QApplication::UnicodeUTF8));
        actionLoadConNodes->setText(QApplication::translate("MainWindow", "LoadConNodes", 0, QApplication::UnicodeUTF8));
        actionSelectNodes->setText(QApplication::translate("MainWindow", "SelectNodes", 0, QApplication::UnicodeUTF8));
        actionSelectTets->setText(QApplication::translate("MainWindow", "SelectTets", 0, QApplication::UnicodeUTF8));
        actionVolMesh->setText(QApplication::translate("MainWindow", "VolMesh", 0, QApplication::UnicodeUTF8));
        actionVolMesh->setShortcut(QApplication::translate("MainWindow", "V", 0, QApplication::UnicodeUTF8));
        actionObjMesh->setText(QApplication::translate("MainWindow", "ObjMesh", 0, QApplication::UnicodeUTF8));
        actionObjMesh->setShortcut(QApplication::translate("MainWindow", "W", 0, QApplication::UnicodeUTF8));
        actionTetGroups->setText(QApplication::translate("MainWindow", "TetGroups", 0, QApplication::UnicodeUTF8));
        actionTetGroups->setShortcut(QApplication::translate("MainWindow", "T", 0, QApplication::UnicodeUTF8));
        actionConNodes->setText(QApplication::translate("MainWindow", "ConNodes", 0, QApplication::UnicodeUTF8));
        actionMaterialGroups->setText(QApplication::translate("MainWindow", "MaterialGroups", 0, QApplication::UnicodeUTF8));
        actionPrepareSimulation->setText(QApplication::translate("MainWindow", "PrepareSimulation", 0, QApplication::UnicodeUTF8));
        actionPrepareSimulation->setShortcut(QApplication::translate("MainWindow", "E", 0, QApplication::UnicodeUTF8));
        actionPauseSimulation->setText(QApplication::translate("MainWindow", "PauseSimulation", 0, QApplication::UnicodeUTF8));
        actionPauseSimulation->setShortcut(QApplication::translate("MainWindow", "Space", 0, QApplication::UnicodeUTF8));
        actionStopSimulation->setText(QApplication::translate("MainWindow", "StopSimulation", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        actionReset->setShortcut(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        actionFixSelectedTets->setText(QApplication::translate("MainWindow", "FixSelectedTets", 0, QApplication::UnicodeUTF8));
        actionSaveMaterial->setText(QApplication::translate("MainWindow", "SaveMaterial", 0, QApplication::UnicodeUTF8));
        actionPrintSelected->setText(QApplication::translate("MainWindow", "PrintSelected", 0, QApplication::UnicodeUTF8));
        actionLoadMaterial->setText(QApplication::translate("MainWindow", "LoadMaterial", 0, QApplication::UnicodeUTF8));
        actionRecord->setText(QApplication::translate("MainWindow", "Record", 0, QApplication::UnicodeUTF8));
        actionRecord->setShortcut(QApplication::translate("MainWindow", "C", 0, QApplication::UnicodeUTF8));
        actionSaveRecordZ->setText(QApplication::translate("MainWindow", "SaveRecordZ", 0, QApplication::UnicodeUTF8));
        actionSaveEigenValues->setText(QApplication::translate("MainWindow", "SaveEigenValues", 0, QApplication::UnicodeUTF8));
        actionSaveEigenVectors->setText(QApplication::translate("MainWindow", "SaveEigenVectors", 0, QApplication::UnicodeUTF8));
        actionClearRecordedZ->setText(QApplication::translate("MainWindow", "ClearRecordedZ", 0, QApplication::UnicodeUTF8));
        actionSaveMaterialAsHinp->setText(QApplication::translate("MainWindow", "SaveMaterialAsHinp", 0, QApplication::UnicodeUTF8));
        actionInterpolateMaterials->setText(QApplication::translate("MainWindow", "InterpolateMaterials", 0, QApplication::UnicodeUTF8));
        actionRecordDisp->setText(QApplication::translate("MainWindow", "RecordDisp", 0, QApplication::UnicodeUTF8));
        actionRecordDisp->setShortcut(QApplication::translate("MainWindow", "C", 0, QApplication::UnicodeUTF8));
        actionClearRecordDisp->setText(QApplication::translate("MainWindow", "ClearRecordDisp", 0, QApplication::UnicodeUTF8));
        actionSaveRecordDisp->setText(QApplication::translate("MainWindow", "SaveRecordDisp", 0, QApplication::UnicodeUTF8));
        actionSaveMtlAsVTK->setText(QApplication::translate("MainWindow", "SaveMtlAsVTK", 0, QApplication::UnicodeUTF8));
        actionSaveRecordVTK->setText(QApplication::translate("MainWindow", "SaveRecordVTK", 0, QApplication::UnicodeUTF8));
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
