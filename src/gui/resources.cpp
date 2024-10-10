/******************************************************************************************
**
** resources.cpp
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
** Created: Sun Oct 22 17:41:53 2023
**
******************************************************************************************/
#include "resources.h"

#include <Kanoop/log.h>

#include <QIcon>
#include <QMutex>

#include <Kanoop/pathutil.h>

const Resources::StandardImageToStringMap Resources::_StandardImageToStringMap;

QMap<int, QString> Resources::_registeredImages;
bool Resources::_standardImagesRegistered = Resources::registerStandardImages();

void Resources::registerImage(int id, const QString &resourcePath)
{
    static QMutex _lock;
    _lock.lock();
    if(_registeredImages.contains(id) && _registeredImages.value(id) != resourcePath) {
        Log::logText(LVL_WARNING, QString("Replacing metadata type %1").arg(resourcePath));
    }
    _registeredImages.insert(id, resourcePath);
    _lock.unlock();

}

QIcon Resources::getIcon(int id)
{
    QIcon result = QIcon(getPixmap(id));
    if(result.isNull()) {
        Log::logText(LVL_DEBUG, QString("No icon found for %1").arg(id));
    }
    return result;
}

QPixmap Resources::getPixmap(int id)
{
    QPixmap result;
    QString resourcePath = _registeredImages.value(id);
    if(resourcePath.isEmpty() == false) {
        result = QPixmap(resourcePath);
        if(result.isNull()) {
            Log::logText(LVL_DEBUG, QString("No registered resource found for %1").arg(id));
        }
    }
    else {
        Log::logText(LVL_DEBUG, QString("No resource Pixmap from %1").arg(resourcePath));
    }
    return result;
}

bool Resources::registerStandardImages()
{
    QList<StandardImage> types = _StandardImageToStringMap.keys();
    for(StandardImage type : types) {
        QString path = QString(":/assets/%1").arg(_StandardImageToStringMap.getString(type));
        Resources::registerImage(type, path);
    }
    return true;
}
