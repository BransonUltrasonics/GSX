/* tgsTimer.h - Tilcon timer class */

/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
29jul15,c_l  support circle,ellipse and arc (V7GFX-247)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines the Tilcon's timer class.

INCLUDE FILES: tilcon/engine/tgsTimer.h
*/

#ifndef _INCLUDE_tgsTimer_H_
#define _INCLUDE_tgsTimer_H_

#include "tilcon/tilcon.h"
#include "tilcon/engine/tgsEventHandler.h"


/* Tilcon's timer class */
class TTimer
    {
    friend class TApplication;
public:
    struct TimeoutEventParam : public EventParam
        {
    public:
        TTimer * source;                /* source which generates this message */
        TRT_S32 elapseSinceTimerStart;  /* elapsed time since timer start */
        TRT_S32 elapseSinceLastTimeout; /* elapsed time since last time-out */
        TRT_S32 timeoutTimes;           /* times of time-out since timer start */
        };

/*******************************************************************************
*
* Timer - construct a timer
*
* SYNOPSIS
* \ss
* TTimer()
* \se
*
* DESCRIPTION
* This function constructs a default timer.  The default timer period is 1000
* mini-seconds and time-out is fired only once.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TTimer() :
        elapse(0),
        timeouts(0),
        lastElapse(0),
        targetSpan(0),
        initDelay(1000),
        repeatDelay(1000),
        repeatTimes(1),
        state(idle)
        {
        }

/*******************************************************************************
*
* Timer - construct a timer with initial values
*
* SYNOPSIS
* \ss
* TTimer
*   (
*   TRT_S32 _intDelay,      /@ intial delay to first timeout @/
*   TRT_S32 _repeatDelay,   /@ reload value @/
*   TRT_S32 _repeatTimes    /@ repeat times @/
*   )
* \se
*
* DESCRIPTION
* This function constructs a default timer.  The default timer period is 1000
* mini-seconds and time-out is fired only once.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TTimer(TRT_S32 _intDelay, TRT_S32 _repeatDelay, TRT_S32 _repeatTimes) :
        elapse(0),
        timeouts(0),
        lastElapse(0),
        targetSpan(0),
        initDelay(_intDelay),
        repeatDelay(_repeatDelay),
        repeatTimes(_repeatTimes),
        state(idle)
        {
        }

/*******************************************************************************
*
* ~Timer - destruct the timer
*
* This function destructs the timer.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TTimer()
        {
        }

/*******************************************************************************
*
* TimeoutEvent - get the time out Event of the timer
*
* This function returns the reference to the time-out event of the time.
* The time-out Event is emitted whenever the timer expires; for repeating timer,
* this event is emitted on every time out. The event parameter is of
* type TimeoutEventParam, which contains the timer source, the elapse time and
* the repeated times:
*
* \cs
* struct TimeoutEventParam : public EventParam
*     {
* public:
*     TTimer * source;                /@ source which generates this message @/
*     TRT_S32 elapseSinceTimerStart;  /@ elapsed time since timer start @/
*     TRT_S32 elapseSinceLastTimeout; /@ elapsed time since last time-out @/
*     TRT_S32 timeoutTimes;           /@ times of time-out since timer start @/
*     }
* \ce
*
* This function is usually used to get the event reference, so a user provided
* event handler can be attached to it.
*
* RETURNS: the reference to the time-out Event.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TEvent & TimeoutEvent()
        {
        return timerEvent;
        }

/*******************************************************************************
*
* SetInitDelay - set the delay time until the first time-out
*
* SYNOPSIS
* \ss
* void SetInitDelay
*     (
*     TRT_S32 value    /@ delay time to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the delay time until the first time-out.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetInitDelay()
*/
    void SetInitDelay
        (
        TRT_S32 value
        )
        {
        initDelay = value;
        }

/*******************************************************************************
*
* GetInitDelay - get the delay time until the first time-out
*
* SYNOPSIS
* \ss
* TRT_S32 GetInitDelay() const
* \se
*
* DESCRIPTION
* This function gets the delay time until the first time-out.
*
* RETURNS: the delay time until the first time-out, in mini-second.
*
* ERRNO: N/A
*
* SEE ALSO: SetInitDelay()
*/
    TRT_S32 GetInitDelay() const
        {
        return initDelay;
        }

/*******************************************************************************
*
* SetRepeatDelay - get period of timer after the first time-out
*
* SYNOPSIS
* \ss
* void SetRepeatDelay
*     (
*     TRT_S32 value    /@ delay time to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the period of timer after the first time-out.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRepeatDelay()
*/
    void SetRepeatDelay
        (
        TRT_S32 value
        )
        {
        repeatDelay = value;
        }

/*******************************************************************************
*
* GetRepeatDelay - get period of timer after the first time-out
*
* SYNOPSIS
* \ss
* TRT_S32 GetRepeatDelay() const
* \se
*
* DESCRIPTION
* This function gets period of timer after the first time-out.
*
* RETURNS: the period of timer after the first time-out, in mini-second.
*
* ERRNO: N/A
*
* SEE ALSO: SetRepeatDelay()
*/
    TRT_S32 GetRepeatDelay() const
        {
        return repeatDelay;
        }

/*******************************************************************************
*
* SetRepeatTimes - set the repeating times of the time event
*
* SYNOPSIS
* \ss
* void SetRepeatTimes
*     (
*     TRT_S32 value    /@ repeating times to be set @/
*     )
* \se
*
* DESCRIPTION
* This function sets the repeating times of the time event.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetRepeatTimes()
*/
    void SetRepeatTimes
        (
        TRT_S32 value
        )
        {
        repeatTimes = value;
        }

/*******************************************************************************
*
* GetRepeatTimes - get the repeating times of the time event
*
* SYNOPSIS
* \ss
* TRT_S32 GetRepeatTimes() const
* \se
*
* DESCRIPTION
* This function gets the repeating times of the time event.
*
* RETURNS: repeating times of the time event
*
* ERRNO: N/A
*
* SEE ALSO: SetRepeatTimes()
*/
    TRT_S32 GetRepeatTimes() const
        {
        return repeatTimes;
        }

/*******************************************************************************
*
* GetElapse - get the elapse since timer start
*
* SYNOPSIS
* \ss
* TRT_S32 GetElapse() const
* \se
*
* DESCRIPTION
* This function gets the repeating times of the time event.
*
* RETURNS: repeating times of the time event
*
* ERRNO: N/A
*
* SEE ALSO: SetRepeatTimes()
*/
    TRT_S32 GetElapse() const
        {
        return elapse;
        }

/*******************************************************************************
*
* Start - start the timer
*
* SYNOPSIS
* \ss
* void Start()
* \se
*
* DESCRIPTION
* This function starts the timer. When a timer is started, the framework steps
* the it in every heart beat of the GUI; if the elapse of the timer reaches
* the time-out limit, its time-out event is triggered.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Stop(), Pause(), Resume()
*/
    void Start();

/*******************************************************************************
*
* Stop - stop the timer
*
* SYNOPSIS
* \ss
* void Stop()
* \se
*
* DESCRIPTION
* This function stops the timer. When a timer is stops, it is removed from the
* watch list of the framework, which means, it will not be stepped and will not
* trigger the time-out event.
*
* Newly constructed timer is in stopped state. A stopped timer is started by
* the Start() method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Start(), Pause(), Resume()
*/
    void Stop();

/*******************************************************************************
*
* Pause - Pause the timer
*
* SYNOPSIS
* \ss
* void Pause()
* \se
*
* DESCRIPTION
* This function pauses the timer. Similar to Stop(), a paused timer is not
* stepped forward by framework, thus will never time-out. The difference to
* Stop() is that the paused timer's elapse and repeated times are not reset,
* and once resumed, they start from the paused values.
*
* Paused timer is started by the Resume() method, not the Start() method.
* Paused timer can enter stop state by the Stop() method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Start(), Pause(), Resume()
*/
    void Pause();

/*******************************************************************************
*
* Resume - Resume the timer
*
* SYNOPSIS
* \ss
* void Resume()
* \se
*
* DESCRIPTION
* This function resumes the paused timer. The Resume() method cannot start a
* stopped timer; stopped timer is started by the Start() method.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Start(), Pause(), Pause()
*/
    void Resume();

private:
    enum State
        {
        idle = 0,
        running,
        paused
        };

    TEvent  timerEvent;

    void Step(TRT_S32 value);

    TRT_S32 elapse;
    TRT_S32 timeouts;
    TRT_S32 lastElapse;
    TRT_S32 targetSpan;

    TRT_S32 initDelay;
    TRT_S32 repeatDelay;
    TRT_S32 repeatTimes;
    State   state;
    };

#endif /* _INCLUDE_tgsTimer_H_ */

