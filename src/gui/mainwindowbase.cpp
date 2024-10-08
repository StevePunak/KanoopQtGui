/******************************************************************************************
**
** mainwindowbase.cpp
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
**
** Author:  Stephen Punak
** Created: Fri Oct 27 09:12:34 2023
**
******************************************************************************************/
#include "guisettings.h"
#include "mainwindowbase.h"

#include <QMdiArea>
#include <QMenu>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QSplitter>
#include <QTimer>

MainWindowBase::MainWindowBase(const QString &loggingCategory, QWidget *parent) :
    QMainWindow{parent},
    LoggingBaseClass(loggingCategory),
    _formLoadComplete(false), _formLoadFailed(false)
{
    MainWindowBase::setObjectName(MainWindowBase::metaObject()->className());
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindowBase::initializeBase()
{
    onPreferencesChanged();

    if(GuiSettings::globalInstance() != nullptr) {
        for(QSplitter* splitter : findChildren<QSplitter*>()) {
            GuiSettings::globalInstance()->restoreLastSplitterState(splitter);
            connect(splitter, &QSplitter::splitterMoved, this, &MainWindowBase::onSpliltterMoved);
        }
    }
}

QMdiArea *MainWindowBase::parentMdiArea()
{
    QMdiArea* result = nullptr;
    QObject* parentWidget = parent();
    while(parentWidget != nullptr && result == nullptr) {
        result = dynamic_cast<QMdiArea*>(parentWidget);
        parentWidget = parentWidget->parent();
    }
    return result;
}

void MainWindowBase::moveEvent(QMoveEvent *event)
{
    if(_formLoadComplete) {
        GuiSettings::globalInstance()->setLastWindowPosition(this, event->pos());
    }
    QMainWindow::moveEvent(event);
}

void MainWindowBase::resizeEvent(QResizeEvent *event)
{
    if(_formLoadComplete) {
        GuiSettings::globalInstance()->setLastWindowSize(this, event->size());
    }
    QMainWindow::resizeEvent(event);
}

void MainWindowBase::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    if(_formLoadComplete == false) {
        if(parentWidget() != nullptr) {
            parentWidget()->setGeometry(QRect(GuiSettings::globalInstance()->getLastWindowPosition(this), GuiSettings::globalInstance()->getLastWindowSize(this)));
        }
        else {
            setGeometry(QRect(GuiSettings::globalInstance()->getLastWindowPosition(this), GuiSettings::globalInstance()->getLastWindowSize(this)));
        }
        _formLoadComplete = true;
        if(_formLoadFailed) {
            QTimer::singleShot(0, parentWidget(), &QWidget::close);
        }
    }
    QMainWindow::showEvent(event);
}

void MainWindowBase::onPreferencesChanged()
{
    if(GuiSettings::globalInstance() == nullptr) {
        return;
    }

    QFont newFont(font());
    int pointSize = GuiSettings::globalInstance()->fontSize();
    newFont.setPointSize(pointSize);
    setFont(newFont);

    // Not sure why menus are still rendered in system font, but we will
    // fix that up here
    QList<QMenu*> menus = findChildren<QMenu*>();
    for(QMenu* menu : menus) {
        menu->setFont(newFont);
    }
}

void MainWindowBase::onSpliltterMoved()
{
    GuiSettings::globalInstance()->saveLastSplitterState(static_cast<QSplitter*>(sender()));
}
