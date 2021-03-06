/******************************************************************************
**
** This file is part of libcommhistory.
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Reto Zingg <reto.zingg@nokia.com>
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License version 2.1 as
** published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
** License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
**
******************************************************************************/

#ifndef COMMHISTORY_GROUPMODEL_P_H
#define COMMHISTORY_GROUPMODEL_P_H

#include <QAbstractItemModel>
#include <QList>
#include <QPair>

#include "groupmodel.h"
#include "eventmodel.h"
#include "group.h"

namespace CommHistory {

class QueryRunner;
class TrackerIO;
class ContactListener;
class CommittingTransaction;
class UpdatesEmitter;

class GroupModelPrivate: public QObject
{
    Q_OBJECT

    Q_DECLARE_PUBLIC(GroupModel);

public:
    GroupModel *q_ptr;

    /*!
     * Model constructor.
     *
     * \param parent Parent object.
     */
    GroupModelPrivate(GroupModel *parent = 0);
    ~GroupModelPrivate();

    QString newObjectPath();

    void addToModel(Group &group);
    void modifyInModel(Group &group, bool query = true);

    bool canFetchMore() const;

    void executeQuery(const QString query);

    CommittingTransaction* commitTransaction(QList<int> groupIds);

    void resetQueryRunner();
    void deleteQueryRunner();

    TrackerIO* tracker();
    void startContactListening();

public Q_SLOTS:
    void eventsAddedSlot(const QList<CommHistory::Event> &events);

    void groupsAddedSlot(const QList<CommHistory::Group> &addedGroups);

    void groupsUpdatedSlot(const QList<int> &groupIds);
    void groupsUpdatedFullSlot(const QList<CommHistory::Group> &groups);

    void groupsDeletedSlot(const QList<int> &groupIds);

    void groupsReceivedSlot(int start, int end, QList<CommHistory::Group> result);

    void modelUpdatedSlot(bool successful);

    void canFetchMoreChangedSlot(bool canFetch);

    void slotContactUpdated(quint32 localId,
                            const QString &contactName,
                            const QList< QPair<QString,QString> > &contactAddresses);

    void slotContactRemoved(quint32 localId);

    void slotContactSettingsChanged(const QHash<QString, QVariant> &changedSettings);

Q_SIGNALS:
    void groupsAdded(const QList<CommHistory::Group> &groups);

    void groupsUpdated(const QList<int> &groupIds);
    void groupsUpdatedFull(const QList<CommHistory::Group> &groups);

    void groupsDeleted(const QList<int> &groupIds);

public:
    EventModel::QueryMode queryMode;
    uint chunkSize;
    uint firstChunkSize;
    int queryLimit;
    int queryOffset;
    bool isReady;
    QList<Group> groups;

    QString filterLocalUid;
    QString filterRemoteUid;

    QueryRunner *queryRunner;
    bool threadCanFetchMore;

    QThread *bgThread;

    TrackerIO *m_pTracker;

    QSharedPointer<ContactListener> contactListener;
    bool contactChangesEnabled;
    QSharedPointer<UpdatesEmitter> emitter;
};

}

#endif
