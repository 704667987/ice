// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef FREEZE_TRANSACTIONI_H
#define FREEZE_TRANSACTIONI_H

#include <Freeze/Transaction.h>
#include <db_cxx.h>

namespace Freeze
{

class ConnectionI;
typedef IceUtil::Handle<ConnectionI> ConnectionIPtr;

class PostCompletionCallback : public virtual IceUtil::Shared
{
public:

    virtual void postCompletion(bool, bool) = 0;
};
typedef IceUtil::Handle<PostCompletionCallback> PostCompletionCallbackPtr;


class TransactionI : public Transaction
{
public:

    virtual void commit();

    virtual void rollback();

    virtual ConnectionPtr getConnection() const;
    
    //
    // Custom refcounting implementation
    //
    virtual void __incRef();
    virtual void __decRef();

    void setPostCompletionCallback(const PostCompletionCallbackPtr&);

    TransactionI(ConnectionI*);
    ~TransactionI();
    
    DbTxn*
    dbTxn() const
    {
        return _txn;
    }

    const ConnectionI*
    getConnectionI() const
    {
        return _connection;
    }

private:

    friend class ConnectionI;
    
    void internalIncRef();
    void internalDecRef();
    
    void postCompletion(bool, bool);

    ConnectionI* const _connection;
    Ice::Int _txTrace;
    DbTxn* _txn;
    PostCompletionCallbackPtr _postCompletionCallback;
};

typedef IceUtil::Handle<TransactionI> TransactionIPtr;

}
#endif
