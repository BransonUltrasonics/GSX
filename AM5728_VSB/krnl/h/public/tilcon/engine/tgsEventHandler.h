/* tgsEventHandler.h - Tilcon event handler class */

/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's event and handler. TEvent class implements
delegation: user can attach handler to TEvent, and when the TEvent is triggered,
it will execute the registered handler. TEvent is widely used in the
framework to let the user register handlers to take care of UI events.

INCLUDE FILES: tilcon/engine/tgsEventHandler.h
*/

#ifndef _INCLUDE_tgsEventHandler_H_
#define _INCLUDE_tgsEventHandler_H_

#include "tilcon/tilcon.h"

/* Tilcon event class */
template<typename M>
class _TEvent
    {
public:

    class THandlerAdaptor
        {
    public:
        THandlerAdaptor(){};
        virtual ~THandlerAdaptor(){};
        virtual void Invoke(M &) const = 0;
        virtual THandlerAdaptor * dup() = 0;
        };

    template<typename C>
    class MemberFuncHandler : public THandlerAdaptor
        {
    public:
        typedef void (C::* FuncType)(M &);

/*******************************************************************************
*
* MemberFuncHandler - construct a member function handler
*
* SYNOPSIS
* \ss
* template<typename C>
* MemberFuncHandler
*     (
*     C * inst,         /@ class instance @/
*     FuncType func     /@ pointer to class member function @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a handler which contains a class instance, and a
* pointer to the class instance's member function.
* The constructed member function handler can be registered to TEvent via
* TEvent's SetHandler() method.
*
* Parameters:
* \is
* \i <inst>
* pointer to the class instance.
* \i <func>
* pointer to the class member function.
* The prototype of class member function is:
* \cs
* void C::func(EventParam &);
* \ce
* The function takes only one parameter; The parameter is of type <EventParam>
* and it contains the information regarding the event that just occurred.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetHanlder()
*/
        MemberFuncHandler(C * _inst, FuncType _func) :
            inst(_inst), func(_func)
            {
            }
        virtual ~MemberFuncHandler()
            {
            }
        virtual void Invoke(M & m) const
            {
            (inst->*func)(m);
            }
        virtual THandlerAdaptor * dup()
            {
            MemberFuncHandler * ptr = new MemberFuncHandler(inst, func);
            return ptr;
            }

    private:
        C *         inst;
        FuncType    func;
        };

    class FuncHandler : public THandlerAdaptor
        {
    public:
        typedef void (* FuncType)(M &, TRT_PVOID);
/*******************************************************************************
*
* FuncHandler - construct a non-member function handler
*
* SYNOPSIS
* \ss
* FuncHandler
*     (
*     FuncType func,    /@ pointer to handler function @/
*     TRT_PVOID param   /@ parameter to be passed to handler function @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a handler which contains a pointer to a function.
* The constructed function handler can be registered to TEvent via TEvent's
* SetHandler() method.
*
* Parameters:
* \is
* \i <func> pointer to the function to be used as handler; the prototype of the
* function is:
* \cs
* void func(EventParam &, TRT_LPVOID);
* \ce
* The function takes two parameters, the first parameter contains the
* information regarding the event that just occurred; the second parameter is
* the <param> argument passed here.
* \i <param>
* optional parameter to be passed as the second parameter when the handler
* function is called.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetHandler()
*/
        FuncHandler
            (
            FuncType _func,
            TRT_PVOID _param
            ) :
            func(_func), param(_param)
            {
            }
        virtual ~FuncHandler()
            {
            }
        virtual void Invoke(M & m) const
            {
            (*func)(m, param);
            }
        virtual THandlerAdaptor * dup()
            {
            FuncHandler * ptr = new FuncHandler(func, param);
            return ptr;
            }
    private:
        FuncType    func;
        TRT_PVOID   param;
        };

    _TEvent() :
        invoker(TRT_NULL)
        {
        };

    ~_TEvent()
        {
        if (invoker)
            delete(invoker);
        };

/*******************************************************************************
*
* SetHandler - set member function handler to the event
*
* SYNOPSIS
* \ss
* template<typename C>
* void SetHandler
*     (
*     const MemberFuncHandler<C> & h    /@ handler to be set @/
*     )
* \se
*
* DESCRIPTION
* This function attaches a MemberFuncHandler structure as the handler of the
* event.
* This function overwrites existing handler if the event already has a handler.
*
* Parameters:
* \is
* \i <h>
* the MemberFuncHandler to be added. See MemberFunctHandler class for detail.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: RemoveHandler(), Execute()
*/
    template<typename C>
    void SetHandler
        (
        const MemberFuncHandler<C> & h
        )
        {
        if (invoker)
            {
            delete invoker;
            invoker = TRT_NULL;
            }
        invoker = new MemberFuncHandler<C>(h);
        }

/*******************************************************************************
*
* SetHandler - set non-member function handler to the event
*
* SYNOPSIS
* \ss
* void SetHandler
*     (
*     const FuncHandler & h    /@ handler to be set @/
*     )
* \se
*
* DESCRIPTION
* This function attaches a none-member function handler to the event.
* This function overwrites existing handler if the event already has a handler.
*
* Parameters:
* \is
* \i <h>
* the FuncHandler to be added. See FunctHandler class for detail.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: RemoveHandler(), Execute()
*/
    void SetHandler
        (
        const FuncHandler & h
        )
        {
        if (invoker)
            {
            delete invoker;
            invoker = TRT_NULL;
            }
        invoker = new FuncHandler(h);
        }

/*******************************************************************************
*
* SetHandler - set member function handler to the event
*
* SYNOPSIS
* \ss
* template<typename C>
* void SetHandler
*     (
*     C * inst,                                     /@ pointer to class @/
*     typename MemberFuncHandler<C>::FuncType func  /@ pointer to function @/
*     )
* \se
*
* DESCRIPTION
* This function attaches a handler to the event. The handler is specified as
* a pointer to class instance and a pointer to the class instance's member
* function.
* This function overwrites existing handler if the event already has a handler.
*
* Parameters:
* \is
* \i <inst>
* pointer to the class instance.
* \i <func>
* pointer to the class member function.
* The prototype of class member function is:
* \cs
* void C::func(EventParam &);
* \ce
* The function takes only one parameter; The parameter is of type <EventParam>
* and it contains the information regarding the event that just occurred.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: RemoveHandler(), Execute()
*/
    template<typename C>
    void SetHandler
        (
        C * _inst,
        typename MemberFuncHandler<C>::FuncType _func
        )
        {
        if (invoker)
            {
            delete invoker;
            invoker = TRT_NULL;
            }
        invoker = new MemberFuncHandler<C>(_inst, _func);
        }

/*******************************************************************************
*
* RemoveHandler - remove the handler from the event
*
* SYNOPSIS
* \ss
* void RemoveHandler()
* \se
*
* DESCRIPTION
* This function removes the handler from the event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetHandler(), Execute()
*/
    void RemoveHandler()
        {
        if (invoker)
            {
            delete invoker;
            invoker = TRT_NULL;
            }
        }

/*******************************************************************************
*
* Execute - execute the event
*
* SYNOPSIS
* \ss
* void Execute
*     (
*     M & param /@ argument to be passed to handler function @/
*     ) const
* \se
*
* DESCRIPTION
* This function executes the event. During the execution the associated handler
* function is invoked, and is given the <param> argument passed in this call.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: SetHandler()
*/
    void Execute(M & param) const
        {
        if (invoker)
            {
            invoker->Invoke(param);
            }
        };

private:
    THandlerAdaptor * invoker;
    };

typedef _TEvent<EventParam>   TEvent;

#endif /* _INCLUDE_tgsEventHandler_H_ */

