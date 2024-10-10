/******************************************************************************************
**
** abstracttreemodel.h
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
** Created: Tue Oct 10 09:37:57 2023
**
******************************************************************************************/
#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H
#include "abstractitemmodel.h"

class LIBKANOOPGUI_EXPORT AbstractTreeModel : public AbstractItemModel
{
    Q_OBJECT
public:
    AbstractTreeModel(QObject* parent = nullptr);
    AbstractTreeModel(const QString& loggingCategory, QObject* parent = nullptr);

protected:
    // AbstractItemModel interface
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual void columnChangedAtRowIndex(const QModelIndex& rowIndex, int columnHeader);
};

#endif // ABSTRACTTREEMODEL_H
