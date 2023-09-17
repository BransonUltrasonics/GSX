/* tgsApplication.h - Tilcon application class */

/*
 * Copyright (c) 2014-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/*
modification history
--------------------
28dec16,yya  fixed draw arc incorrectly (V7GFX-351)
27dec16,yya  deleted redundant variable (V7GFX-358)
14mar16,jnl  support window surface (F6570)
09jul15,c_l  support IPC (V7GFX-248)
01jan14,yya  created (US15550)
*/

/*
DESCRIPTION

This file defines the tilcon's application class.

TApplication class is the basis for any Tilcon application. This class
encapsulates all vital services for running a GUI application, such as, display,
input, font, render, visual style, timer etc.

\sh Creation & Destruction

For any Tilcon application, the first thing to do is to instantiate a
TApplication class, during which the necessary services are up and running, then
the user can begin other initialization work and proceed to the main UI logic.
Similarly, during the program termination, the TApplication instance shall be
the last to get de-initialized, after all other components and widgets are done
de-initializing; this sequence ensures that the underlying services are still
present in case the other parts of the program need to access them in their
de-initialization process.

TApplication has two constructors: one accepts minimum parameters just enough
to initiate a basic Tilcon kernel; The other one accepts the <TAppParam>
data type, which is a helper class to handle additional arguments passed to the
TApplication instance to be created, such as, specifying the IO device names of
the display device and the input device. Following code demonstrate this method:

\cs

/@ program entry @/
void main()
    {
    /@ create a TApplication instance @/
    TApplication * myApp = new TApplication("-name myApp -geometry 1024x768 "
        "-display /dev/fb0 -input /input/event");

    /@ body of my UI @/
    MyUI();

    /@ destruction @/
    delete myApp;
    }
\ce

The previous example also shows the recommended practice of the creation and
the destruction of the TApplication instance: the TApplication instance shall
be allocated at the very beginning of the program, and is freed right before the
program termination. Other practices, such as declaring TApplication instance as
a global variable is not recommended, because the initialization sequence of
global variable (as well as static class members) is not determined, thus
it is unable to ensure that TApplication's instance is created before other
instances that may rely on the presence of TApplication to properly work.
For the same reason, we do not recommend any use of global variables (as well as
static class members) unless they are POD (Plain Old Data) types.

\sh Error Handling

The TApplication's constructor may encounter error during the initialization
process, for example, failure to open the display device or input device, or out
of memory in some extreme circumstances; in such case, the constructor of
TApplication throws an exception to notify such unfortunate occurrence:

\is
\i <more than one application instance>
TException with type ET_second_application_instance when attempting to
create more than one application instance;
\i <platform initialization failure>
TException with type ET_platform_init when initialization of underlying
platform fails;
\i <memory allocation failure>
std::bad_alloc when memory allocation failure;
\ie

As a result, to catch such errors reported via exceptions, the TApplication's
constructor shall be put inside the try-catch block, as shown blow:

\cs

/@ program entry @/
void main()
    {
    TApplication * myApp = TRT_NULL;

    /@ try initialize the application @/
    try
        {
        /@ construct @/
        myApp = new TApplication("myApp");

        /@ body of my UI @/
        MyUI();

        /@ destruct @/
        delete myApp;
        }

    /@ excetption defined by tilcon @/
    catch (TException e)
        {
        cout << "Tilcon exception:" << e.GetType() << endl;
        }

    /@ memory allocation @/
    catch (std::bad_alloc)
        {
        cout << "bad_alloc" << endl;
        }

    /@ other types of error @/
    catch (...)
        {
        cout << "general error" << endl;
        }
    }
\ce

If the user application does not want to deal with exception, the alternative
is to use the TApplication::Create() helper function to construct TApplication
instance; This helper function traps all exception during creation of the
TApplication instance, and return a NULL pointer to caller if the construction
fails. TApplication instance created by Create() shall be destroyed by
TApplication::Destroy() method. Following code shows how to use the
TApplication::Create() and TApplication::Destroy():

\cs

/@ program entry @/
void main()
    {
    /@ construct the application @/
    TApplication * app = TApplication::Create("myApp");

    /@ check the construction result @/
    if (app != TRT_NULL)
        {
        /@ body of my UI @/
        MyUI();
        }

    /@ destroy application instance, it's OK to pass TRT_NULL to it @/
    TApplication::Destroy(app);
    }

\ce

\sh Execution Control

TApplication's execution is controlled by Run() and Stop() method. The Run()
is typically called after the initialization and building of UI is completed.
Calling Run() makes the execution enters the message loop, where the framework
is reading messages from the underlying platform and distribute them among the
UI hierarchy. The Run() method only returns when the Stop() method is called or
a QUIT message is encountered in the message loop.

by calling Run(), the user effectively gives up the control of the application
to the Tilcon Framework, and is only notified when certain events occurs via
registered Event Handlers. Only when the Run() returns (as a result of the
Stop() or a QUIT message) the control of application is returned to the user.
This realizes the Inversion of Control (IoC) method of UI application.

\sh Application Messages

By default the Tilcon Framework reads application messages from the underlying
platform, then process and distribute them accordingly. The TApplication's
SendMessage() method allows the user to send message to the Tilcon Framework,
for example, a keyboard message from a on-screen virtual keypad, or a QUIT
message to terminate the application. The TApplication's RegisterMessageHook()
method allows the user to provide a hook function, which will be called before
the Framework starts to process message. The user can use the message hook
function to log, filter or alter the messages prior to the framework's message
handling logic.

\sh Application-Wide Widget Property

TApplication offers methods to manage property of certain widget type within
the scope of the whole application. RegisterProperty() method registers a
customized property for a given widget type. After the registration, all newly
created widget of that particular type will apply the registered property.
UnregisterProperty() will remove the registered property for a given widget
type, and widget of that type subsequently created uses default property.

The properties of widget that can be customized typically includes visual
appearance, layout, interactive behaviours, etc. Please refer to the
description of individual widget as to what properties can be customized
for that widget.

This feature is typically used to implement a customized UI style, e.g., to
resemble certain desktop OS flavour, or a personalized theme.

\sh Query Information

TApplication offers method for querying application-wide information,
GetDesktopRect() method returns the screen's bounding rectangle.
GetUserParam() method returns the input parameters passed by the user to
construct the application instance.

INCLUDE FILES: tilcon/engine/tgsApplication.h
*/

#ifndef _INCLUDE_tgsApplication_H_
#define _INCLUDE_tgsApplication_H_

/* includes */

#include "tilcon/engine/tgsControl.h"
#include "tilcon/engine/tgsWindow.h"
#include "tilcon/engine/tgsPainter.h"
#include "tilcon/engine/tgsTimer.h"
#include "tilcon/engine/tgsVisual.h"
#include "tilcon/engine/tgsPixelBuffer.h"

class TSurface;
class TWSHost;
class TWSWindow;
class TPainter;
class TVisualEngine;
class TTtfFontEngine;
class TBmfFontEngine;
class TDefaultVisualEngine;
class TClassicVisualEngine;

/* arguments passed to platform engine */

class TAppParam
    {
public:
/*******************************************************************************
*
* TAppParam - construct a TAppParam class instance
*
* SYNOPSIS
* \ss
* TAppParam()
* \se
*
* DESCRIPTION
* This constructor creates a default TAppParam class. The default values of
* the class members are shown below:
*
* \is
* \i <name>
* the default name is "defaultApplication".
* \i <screenSize>
* the default screen size is TSize(0, 0).
* \i <displayDevName>
* the default display device name is "/dev/fb0".
* \i <inputDevName>
* the default input device name is "/input/event".
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    explicit TAppParam();

/*******************************************************************************
*
* TAppParam - construct a TAppParam class instance
*
* SYNOPSIS
* \ss
* TAppParam
*   (
*   TRT_LPCSTR  cmd     /@ string containing the parameters to be used @/
*   )
* \se
*
* DESCRIPTION
* This constructor creates a TAppParam class with the specified parameter
* values passed as a C-style string. The parameter values are specified as
* name and value pairs inside the string, for example, the following string
* specifies the application name to be "myApp", the screen size is 1280x1024,
* and the display device name is "/dev/fb1":
*   "-name myApp -geometry 1280x1024 -display /dev/fb1"
*
* The options that can be specified include:
* \is
* \i <-name>
* specify the name of the application.
* \i <-geometry>
* specify the screen size of the application.
* \i <display>
* specify the display name of the application.
* \i <input>
* specify the input name of the application.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    explicit TAppParam(TRT_LPCSTR);

/*******************************************************************************
*
* TAppParam - construct a TAppParam class instance
*
* SYNOPSIS
* \ss
* TAppParam
*   (
*   int     argc,   /@ number of input parameters @/
*   char *  argv[]  /@ string array of the input parameters @/
*   )
* \se
*
* DESCRIPTION
* This constructor creates a TAppParam class with the specified parameter
* values passed as string array. This constructor is intended to be used to
* parse the command line option from the program entry.
*
* The options that can be specified include:
* \is
* \i <-name>
* specify the name of the application.
* \i <-geometry>
* specify the screen size of the application.
* \i <display>
* specify the display name of the application.
* \i <input>
* specify the input name of the application.
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    explicit TAppParam(int argc, char * argv[]);

/*******************************************************************************
*
* ~TAppParam - destruct the TAppParam class instance
*
* This function destroys the TAppParam instance
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TAppParam();

/*******************************************************************************
*
* Add - add additional parameters
*
* SYNOPSIS
* \ss
* Add
*   (
*   TRT_LPCSTR  cmd  /@ the string containing the parameters @/
*   )
* \se
*
* DESCRIPTION
* This function adds additional parameters to the TAppParam instance. The added
* parameters are specified in the C-style string See the TAppParam's constructor
* for the options that can be specified.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Add(TRT_LPCSTR);

/*******************************************************************************
*
* Add - add additional parameters
*
* SYNOPSIS
* \ss
* Add
*   (
*   int     argc,   /@ number of input parameters @/
*   char *  argv[]  /@ string array of the input parameters @/
*   )
* \se
*
* DESCRIPTION
* This function adds additional parameters to the TAppParam instance. The added
* parameters are specified in the string array. This function is intended to be
* used to parse the command line option from the program entry. See the
* TAppParam's constructor for the options that can be specified.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void Add(int argc, char * argv[]);

    TString     name;
    TSize       screenSize;
    TString     displayDeviceName;
    TString     inputDeviceName;
    };

/* platform information reported by platform engine */

struct TScreenInfo
    {
    TRect       rect;   /* physical screen size */
    TRT_S32     dpi;    /* screen dpi */
    };

/* paint interface */

class TDisplay
    {
public:

    /* pixel buffer */

    virtual TRT_PBUFFER_HANDLE CreatePixelBuffer(TRT_S32 w, TRT_S32 h) = 0;
    virtual void DestroyPixelBuffer(TRT_PBUFFER_HANDLE) = 0;
    virtual TSize GetPixelBufferSize(TRT_PBUFFER_HANDLE) = 0;
    virtual void CopyPixelBuffer(TRT_PBUFFER_HANDLE dst, const TPoint & dstOffset,
        TRT_PBUFFER_HANDLE src, const TPoint & srcOffset, const TSize & size) = 0;
    virtual void ReadPixelBuffer(TRT_PBUFFER_HANDLE src,
        const TPoint & offset, const TSize & size,
        TRT_BYTE * buffer, ColorFormat bufferFormat, TRT_S32 bufferStride) = 0;
    virtual void WritePixelBuffer(TRT_PBUFFER_HANDLE dst,
        const TPoint & offset, const TSize & size,
        const TRT_BYTE * buffer, ColorFormat bufferFormat, TRT_S32 bufferStride) = 0;

    /* path */

    virtual TRT_PATH_HANDLE CreatePath() = 0;
    virtual void DestroyPath(TRT_PATH_HANDLE) = 0;
    virtual void ClearPath(TRT_PATH_HANDLE) = 0;
    virtual void AppendDataToPath(TRT_PATH_HANDLE, TRT_U32, const TRT_BYTE *, const TRT_FLOAT *) = 0;
    virtual void AppendPathToPath(TRT_PATH_HANDLE, TRT_PATH_HANDLE) = 0;
    virtual void TransformPath(TRT_PATH_HANDLE, const TMatrix3x3 &) = 0;
    virtual TRectF GetPathBoundRect(TRT_PATH_HANDLE) = 0;

    /* font */

    virtual TRT_STATUS RegisterTtfFont(const TTtfFont &) = 0;
    virtual TRT_FONT_HANDLE CreateTtfFont(const TTtfFont &) = 0;
    virtual void DestroyTtfFont(TRT_FONT_HANDLE) = 0;
    virtual void GetTtfMetrics(TRT_FONT_HANDLE, const TFont &, FontMetrics &) = 0;
    virtual TRT_S32 MeasureTtfWidth(TRT_FONT_HANDLE, const TFont &, TRT_LPCWSTR, TRT_S32) = 0;

    virtual TRT_STATUS RegisterBmfFont(const TBmfFont &) = 0;
    virtual TRT_FONT_HANDLE CreateBmfFont(const TBmfFont &) = 0;
    virtual void DestroyBmfFont(TRT_FONT_HANDLE) = 0;
    virtual void GetBmfMetrics(TRT_FONT_HANDLE, const TFont &, FontMetrics &) = 0;
    virtual TRT_S32 MeasureBmfWidth(TRT_FONT_HANDLE, const TFont &, TRT_LPCWSTR, TRT_S32) = 0;

    /* paint */

    virtual TRT_HANDLE CreatePaintContext() = 0;
    virtual void DestroyPaintContext(TRT_HANDLE) = 0;
    virtual TRT_STATUS BeginPaint(TRT_HANDLE, TRT_PBUFFER_HANDLE) = 0;
    virtual void EndPaint(TRT_HANDLE) = 0;

    /* scalar painter state */

    virtual void SetViewport(TRT_HANDLE, const TPoint & origin, const TRect & bound) = 0;
    virtual void SetClipRect(TRT_HANDLE, const TRect &) = 0;
    virtual void ClearClipRect(TRT_HANDLE) = 0;
    virtual void SetPen(TRT_HANDLE, const TPen &) = 0;
    virtual void SetBrush(TRT_HANDLE, const TBrush &) = 0;
    virtual void SetFont(TRT_HANDLE, const TFont &) = 0;
    virtual void SetTextColor(TRT_HANDLE, const TColor &) = 0;
    virtual void SetRasterMode(TRT_HANDLE, RasterMode) = 0;

    /* vector painter state */

    virtual void SetPathWidth(TRT_HANDLE, TRT_FLOAT) = 0;
    virtual void SetFillRule(TRT_HANDLE, FillRule) = 0;
    virtual void SetBlendMode(TRT_HANDLE, BlendMode) = 0;
    virtual void SetDashStyle(TRT_HANDLE, DashStyle) = 0;
    virtual void SetJointStyle(TRT_HANDLE, JointStyle) = 0;
    virtual void SetCapStyle(TRT_HANDLE, CapStyle) = 0;
    virtual void SetFillPaint(TRT_HANDLE, const TPaint &) = 0;
    virtual void SetStrokePaint(TRT_HANDLE, const TPaint &) = 0;
    virtual void SetMatrix(TRT_HANDLE, const TMatrix3x3 &) = 0;
    virtual void SetPaintMatrix(TRT_HANDLE, const TMatrix3x3 &) = 0;

    /* scalar painter operations */

    virtual void DrawLine(TRT_HANDLE, const TPoint &, const TPoint &) = 0;
    virtual void DrawRect(TRT_HANDLE, const TRect &, TRT_BOOL) = 0;
    virtual void DrawUCSText(TRT_HANDLE, TRT_LPCWSTR, TRT_S32, TRT_S32, TRT_S32) = 0;
    virtual TSize MeasureUCSText(TRT_HANDLE, TRT_LPCWSTR, TRT_S32) = 0;
    virtual void DrawImage(TRT_HANDLE, const TImage &, TRT_S32, TRT_S32) = 0;
    virtual void DrawPixelBuffer(TRT_HANDLE, TRT_PBUFFER_HANDLE, TRT_S32, TRT_S32) = 0;
    virtual void DrawCircle(TRT_HANDLE, const TRect &) = 0;
    virtual void DrawEllipse(TRT_HANDLE, const TRect &) = 0;
    virtual void DrawArc(TRT_HANDLE, const TRect &, const TPoint &, const TPoint &) = 0;
    virtual void DrawPie(TRT_HANDLE, const TRect &, const TPoint &, const TPoint &) = 0;

    /* vector painter operations */

    virtual void StrokePath(TRT_HANDLE, TRT_PATH_HANDLE hp) = 0;
    virtual void StrokePath(TRT_HANDLE, const TPointF *, TRT_U32, TRT_BOOL) = 0;
    virtual void FillPath(TRT_HANDLE, TRT_PATH_HANDLE hp) = 0;
    virtual void FillPath(TRT_HANDLE, const TPointF *, TRT_U32) = 0;

    /* paint copy content */

    virtual void CopyToPixelBuffer(TRT_HANDLE, TRT_PBUFFER_HANDLE dst,
        TRT_S32 px, TRT_S32 py, TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h) = 0;
    virtual void CopyFromPixelBuffer(TRT_HANDLE, TRT_PBUFFER_HANDLE src,
        TRT_S32 px, TRT_S32 py, TRT_S32 x, TRT_S32 y, TRT_S32 w, TRT_S32 h) = 0;
    };

/* platform interface */

class TPlatform
    {
public:
    /* screen */

    virtual void GetScreenInfo(TScreenInfo &) = 0;
    virtual void SaveSurface(TRT_PBUFFER_HANDLE, const TPoint &, const TPoint &, const TSize &) = 0;
    virtual void BlitSurface(TRT_PBUFFER_HANDLE, const TPoint &, const TPoint &, const TSize &) = 0;
    virtual void PostScreen(TRT_S32, const TRT_S32 *) = 0;
    virtual void SwapScreen() = 0;

    /* message */

    virtual TRT_STATUS WaitMessage(TRT_APP_MSG & appMsg, TRT_BOOL block) = 0;
    virtual TRT_STATUS SendMessage(const TRT_APP_MSG & appMsg, TRT_BOOL block) = 0;
    virtual TDisplay & GetDisplay() = 0;
    };

/* visual interface */

class TVisualEngine
    {
public:
    virtual void DrawFrame(FrameType, TPainter &, const VisualState &, const TRect &) = 0;
    };

typedef TRT_BOOL (*MessageHookFunc)(TRT_PVOID, TRT_APP_MSG &);

/* Tilcon application class */

class TApplication
    {
    friend class TWindow;
    friend class TPath;
public:
/*******************************************************************************
*
* Create - create a Tilcon application
*
* SYNOPSIS
* \ss
* static TApplication * Create
*     (
*     const TAppParam & param   /@ optional parameters @/
*     )
* \se
*
* DESCRIPTION
* This function created an TApplication instance. The function is a wrapper to
* the TApplication's constructor; this function traps possible exception
* thrown during the creation of TApplication, and returns NULL to caller.
*
* This function is provided to users who does not want do deal with exception,
* but prefer the method of checking return values.
*
* Refer to the TApplication() constructor for description of input parameters.
*
* RETURNS: pointer to TApplication instance when successful; otherwise TRT_NULL.
*
* ERRNO: N/A
*
* SEE ALSO: Destroy()
*/
    static TApplication * Create
        (
        const TAppParam & param
        );

/*******************************************************************************
*
* Destroy - destroy the Tilcon application
*
* SYNOPSIS
* \ss
* static void Destroy
*     (
*     TApplication * app /@ application to be destroyed @/
*     )
* \se
*
* DESCRIPTION
* This function destroys the TApplication instance crated by Create() method.
* This function should be paired with the Create() method, It is not allowed
* to use this function to destroy TApplication instance created by means other
* than Create(), i.e., by allocating using "new" operation.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Create()
*/
    static void Destroy
        (
        TApplication * app
        );

/*******************************************************************************
*
* TApplication - construct a Tilcon application
*
* SYNOPSIS
* \ss
* TApplication
*     (
*     const TString & name = "",    /@ application identifier @/
*     TRT_U32 w = 0,                /@ screen width @/
*     TRT_U32 h = 0                 /@ screen height @/
*     )
* \se
*
* DESCRIPTION
* This constructor creates a TApplication instance. TApplication is a singleton
* class, which means there shall always be only one class instance. So if a
* second TApplication class is to be constructed, a TException with type
* ET_second_application_instance is thrown.
*
* Parameters:
* \is
* \i <name>
* specify application's name;
* \i <w>, <h>
* specify the virtual screen size of the application, which may not be the same
* as the physical screen size, and in such case, the virtual screen is aligned
* at the upper left corner of the physical screen; If either <w> or <h> is
* specified as 0, then the virtual screen size will be equal to that of
* the physical screen.
* \ie
*
* the constructor throws following types of exceptions:
* \is
* \i <more than one application instance>
* TException with type ET_second_application_instance when attempting to
* create more than one application instance;
* \i <platform initialization failure>
* TException with type ET_platform_init when initialization of underlying
* platform fails;
* \i <memory allocation failure>
* std::bad_alloc when memory allocation failure;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Create(), Destroy()
*/
    explicit TApplication
        (
        const TString & name = "",
        TRT_U32 w = 0,
        TRT_U32 h = 0
        );

/*******************************************************************************
*
* TApplication - construct a Tilcon application
*
* SYNOPSIS
* \ss
* TApplication
*     (
*     const TAppParam & param   /@ optional parameters @/
*     )
* \se
*
* DESCRIPTION
* This constructor creates a TApplication instance with additional options.
* TApplication is a singleton class, which means there shall always be only one
* class instance. So if a second TApplication class is to be constructed,
* a TException with type ET_second_application_instance is thrown.
*
* Parameters:
* \is
* \i <param>
* the TAppParam helper class containing additional options to create the
* TApplication instance. Refer to the TAppParam class for how to construct the
* parameter class.
* \ie
*
* the constructor throws following types of exceptions:
* \is
* \i <more than one application instance>
* TException with type ET_second_application_instance when attempting to
* create more than one application instance;
* \i <platform initialization failure>
* TException with type ET_platform_init when initialization of underlying
* platform fails;
* \i <memory allocation failure>
* std::bad_alloc when memory allocation failure;
* \ie
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Create(), Destroy()
*/
    explicit TApplication
        (
        const TAppParam & param
        );

/*******************************************************************************
*
* ~TApplication - destruct the Tilcon application
*
* This function destroys the TApplication instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TApplication();

/*******************************************************************************
*
* Instance - get the singleton Tilcon application class instance
*
* SYNOPSIS
* \ss
* static TApplication & Instance()
* \se
*
* DESCRIPTION
* This function gets the reference to the singleton TApplication class instance.
* If no TApplication has been instantiated yet, a TException with type
* ET_no_application_instance is thrown.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    static TApplication & Instance();

/*******************************************************************************
*
* Run - run the application
*
* SYNOPSIS
* \ss
* TRT_S32 Run()
* \se
*
* DESCRIPTION
* This function enters the message loop and only return until Stop() is called
* or a QUIT message is sent to the application's message queue by the
* TApplication's SendMessage() method.
*
* RETURNS: exit code which is passed by the Stop() or SendMessage().
*
* ERRNO: N/A
*
* SEE ALSO: Stop(), SendMessage()
*/
    TRT_S32 Run();

/*******************************************************************************
*
* Stop - stop the application
*
* SYNOPSIS
* \ss
* void Stop
*     (
*     TRT_S32 exitCode  /@ exit code to be returned by Run() @/
*     )
* \se
*
* DESCRIPTION
* This function signals the application to exit message loop, i.e., it causes
* the Run() method to return to its caller. If the Application is not started
* yet, i.e., not call the Run() to enter the loop, then Stop() has no effect.
*
* <exitCode> is the value which will be returned by the Run() method. User can
* pass different exit code to represent different user-defined reasons for
* application termination.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: Run()
*/
    void Stop
        (
        TRT_S32 eixtCode = 0
        );

/*******************************************************************************
*
* SendMessage - send a message to the application
*
* SYNOPSIS
* \ss
* TRT_STATUS SendMessage
*     (
*     const TRT_APP_MSG &   msg,    /@ message to be sent @/
*     TRT_BOOL              block   /@ block if message queue is full @/
*     )
* \se
*
* DESCRIPTION
* This function sends a message to the application message loop.
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_STATUS SendMessage
        (
        const TRT_APP_MSG & msg,
        TRT_BOOL            block
        );

/*******************************************************************************
*
* RegisterMessageHook - register a hook function to process application message
*
* SYNOPSIS
* \ss
* void RegisterMessageHook
*     (
*     MessageHookFunc hook, /@ pointer to hook function @/
*     TRT_PVOID param       /@ parameter to be passed when hook is called @/
*     )
* \se
*
* DESCRIPTION
* This function registers a hook which will process the message before the
* application's message handling logic. The hook function returns value type of
* TRT_BOOL, indicating if the message is to be further processed, i.e., if the
* hook returns TRT_FALSE, then the message will be discarded (i.e., not reach
* the application's message handling logic); if it returns TRT_TRUE, the
* messages is routed as usual.
*
* The message hook function can be used to change, filter or log incoming
* application message, e.g. to implement Input Method or Testing Simulation.
*
* The message hook function's prototype is:
* \cs
* TRT_BOOL MessageHookFunc(TRT_PVOID param, TRT_APP_MSG & msg);
* \ce
* The hook function takes two parameters, the first one is the user-specified
* TRT_LPVOID type which is given by the second parameter of the
* RegisterMessageHook(); the second one is the TRT_APP_MSG to be processed.
* The return value of the hook function indicates whether the message shall be
* further processed by the framework, or be discarded.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void RegisterMessageHook
        (
        MessageHookFunc hook,
        TRT_PVOID param
        );

/*******************************************************************************
*
* RegisterTtfFont - register a TTF font to the application
*
* SYNOPSIS
* \ss
* TRT_STATUS RegisterTtfFont
*     (
*     const TTF_FONT_DESC * desc   /@ ttf font descriptor @/
*     )
* \se
*
* DESCRIPTION
* This function registers a vector font to the application.
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: RegisterBmfFont()
*/
    TRT_STATUS RegisterTtfFont(const TTF_FONT_DESC *);

/*******************************************************************************
*
* RegisterBmfFont - register a BMF font to the application.
*
* SYNOPSIS
* \ss
* TRT_STATUS RegisterBmfFont
*     (
*     const UGL_BMF_FONT_DESC * desc   /@ BMF font descriptor @/
*     )
* \se
*
* DESCRIPTION
* This function registers a BMF font to the application.
*
* RETURNS: TRT_STATUS_OK if successful; otherwise TRT_STATUS_ERROR.
*
* ERRNO: N/A
*
* SEE ALSO: RegisterVectorFont()
*/
    TRT_STATUS RegisterBmfFont(const UGL_BMF_FONT_DESC *);

/*******************************************************************************
*
* RegisterProperty - register a property for the given control type
*
* SYNOPSIS
* \ss
* TRT_STATUS RegisterProperty
*     (
*     ControlType type,                 /@ type of control @/
*     const ControlProperty * property  /@ property to be registered @/
*     )
* \se
*
* DESCRIPTION
* This function registers a property for a given control type. After the
* registration, all controls of this type will apply the registered property
* when they are created. This function is often used to implement a customized
* style for UI.
*
* The registered property is only applied when the control with the given type
* is being created. Users can freely change the attributes of the control after
* the control is created.
*
* Only one property is allowed to be registered for a control type at one time;
* this function returns TRT_STATUS_ERROR if there is already one property for
* the given control type. UnregisterProperty() method can be used to remove
* previously registered property.
*
* The framework does not maintain any copy of the registered property, so the
* caller of this function shall make sure that the passed property pointer
* <property> is valid for the framework to access, until UnregisterProperty()
* method is called which stops the framework to further reference this pointer.
*
* Following example registered a customized check box style to the application,
*
* \cs
* /@ create a customized check box property @/
* TCheckBox::Property * p = new TCheckBox::Property;
*
* /@ change the property to show check box's indicator at right side @/
* p->indicatorAtLeft = TRT_FALSE;
*
* /@ register the property to application @/
* app->RegisterProperty(CT_check_box, p);
* \ce
*
* After executing above code, all check-box created afterwards shows the check
* mark at the right side.
*
* Parameters:
* \is
* \i <type>
* the control type for which the property is to be registered.
* \i <property>
* the pointer to property to be registered.
* \ie
*
* RETURNS: TRT_STATUS_OK if the property for the given control type is
* successfully registered.
* TRT_STATUS_ERROR if registration fails, possible reasons are:
* there is already one registered property for the given control type;
* the provided property structure does not match the provided control type;
*
* ERRNO: N/A
*
* SEE ALSO: UnregisterProperty(), GetProperty()
*/
    TRT_STATUS RegisterProperty
        (
        ControlType type,
        const ControlProperty * property
        );

/*******************************************************************************
*
* UnregisterProperty - remove the registered property for the given control type
*
* SYNOPSIS
* \ss
* TRT_STATUS UnregisterProperty
*     (
*     ControlType type      /@ type of control @/
*     )
* \se
*
* DESCRIPTION
* This function remove the property of certain control type, which was
* previously registered to application by the RegisterProperty() method.
*
* RETURNS: TRT_STATUS_OK if the property for the given control type is
* successfully unregistered.
* TRT_STATUS_ERROR if operation fails (there is no registered property for the
* given control type).
*
* ERRNO: N/A
*
* SEE ALSO: RegisterProperty(), GetProperty()
*/
    TRT_STATUS UnregisterProperty
        (
        ControlType type
        );

/*******************************************************************************
*
* GetProperty - get the registered property for a given control type
*
* SYNOPSIS
* \ss
* const ControlProperty * GetProperty
*     (
*     ControlType type  /@ control type of which to request property @/
*     ) const
* \se
*
* DESCRIPTION
* This function retrieves the registered property for a given control type.
*
* RETURNS: TRT_NULL if no property is registered for the given control;
* otherwise the pointer to the registered property.
*
* ERRNO: N/A
*
* SEE ALSO: RegisterProperty(), UnregisterProperty()
*/
    const ControlProperty * GetProperty
        (
        ControlType type
        ) const;

/*******************************************************************************
*
* GetDesktopRect - get the desktop rectangle
*
* SYNOPSIS
* \ss
* TRect GetDesktopRect() const
* \se
*
* DESCRIPTION
* This function gets the desktop rectangle.
*
* RETURNS: the desktop rectangle.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRect GetDesktopRect() const
        {
        return screenInfo.rect;
        }

    TWindow & GetDesktopWindow()
        {
        return *TWindow::root;
        }

/*******************************************************************************
*
* GetAppParam - get the parameters used to construct the application
*
* SYNOPSIS
* \ss
* const TAppParam & GetAppParam() const
* \se
*
* DESCRIPTION
* This function gets the parameters which are passed to the constructor of the
* TApplication class.
*
* RETURNS: reference to TAppParam data type.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    const TAppParam & GetAppParam() const
        {
        return appParam;
        }

/*******************************************************************************
*
* ShowCursor - show or hide cursor on the screen
*
* SYNOPSIS
* \ss
* void ShowCursor
*     (
*     TRT_BOOL show /@ show or hide cursor @/
*     )
* \se
*
* DESCRIPTION
* This function shows or hides cursor on the screen.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    void ShowCursor(TRT_BOOL);

/*******************************************************************************
*
* SetCustomVisualEngine - installs customized visual engine
*
* SYNOPSIS
* \ss
* void SetCustomVisualEngine
*     (
*     TVisualEngine * engine    /@ visual engine to be installed @/
*     )
* \se
*
* DESCRIPTION
* This function installs a <TVisualEngine> to be used to draw UI elements.
* The user installed <TVisualEngine> overwrites the default <TVisualEngine>
* interface. The application only reference the install visual by pointer, so
* the caller shall make sure the installed engine remains valid until
* application termination or a new visual engine is installed to replace it.
* Passing TRT_NULL to SetCustomVisualEngine() removes the current engine.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO: GetCustomVisualEngine()
*/
    void SetCustomVisualEngine(TVisualEngine * engine)
        {
        customVisualEngine = engine;
        }

/*******************************************************************************
*
* GetCustomVisualEngine - get the user installed visual engine
*
* SYNOPSIS
* \ss
* const TVisualEngine * GetCustomVisualEngine() const
* \se
*
* DESCRIPTION
* This function gets the user installed <TVisualEngine>. The user installed
* <TVisualEngine> overwrites the default <TVisualEngine>, and provides Tilcon
* application with customized ways to draw UI elements, such as frames, button,
* indicators, etc. See <TVisualEngine> class for more information.
*
* RETURNS: the user installed <TVisualEngine> interface.
*
* ERRNO: N/A
*
* SEE ALSO: SetCustomVisualEngine()
*/
    const TVisualEngine * GetCustomVisualEngine() const
        {
        return customVisualEngine;
        }

    void GuiHeartBeat(TRT_S32 elapse);
    TRT_STATUS AddTimer(TTimer *);
    TRT_STATUS RemoveTimer(TTimer *);
    TRT_STATUS AddHost(TWindow *);
    TRT_STATUS RemoveHost(TWindow *);

    static TPlatform & GetPlatform()
        {
        return *TApplication::Instance().platform;
        }
    static TDisplay & GetDisplay()
        {
        return (*TApplication::Instance().platform).GetDisplay();
        }
    static TTtfFontEngine & GetTtfFontEngine()
        {
        return *TApplication::Instance().ttfFontEngine;
        }
    static TBmfFontEngine & GetBmfFontEngine()
        {
        return *TApplication::Instance().bmfFontEngine;
        }

private:

    enum RunState
        {
        RunState_stopped = 0,
        RunState_running
        } runState;

    friend class TVisual;

    TVisualEngine * GetVisualEngine();
    void StepTimers(TRT_S32 elapse);
    void UpdateWindows();
    TRT_STATUS WaitMessage(TRT_APP_MSG &, TRT_BOOL);
    void ProcessMessage(TRT_APP_MSG &);
    void CreatePrivate();
    void DestroyPrivate();

private:
    static TApplication *               application;

    TAppParam                           appParam;
    TScreenInfo                         screenInfo;
    TRT_S32                             exitCode;

    TArray<const ControlProperty *>     propertyList;
    TArray<TTimer *>                    timerList;
    TArray<TWindow *>                   hostList;

    MessageHookFunc                     msgHookFunc;
    TRT_PVOID                           msgHookParam;

    TPoint                              cursorPt;
    TPoint                              newCursorPt;
    TSize                               cursorSize;
    TPixelBuffer *                      cursorBuffer;
    TPixelBuffer *                      cursorSave;
    TPixelBuffer *                      cursorTemp;
    int                                 showCursor;

    TPlatform *                         platform;
    TTtfFontEngine *                    ttfFontEngine;
    TBmfFontEngine *                    bmfFontEngine;
    TDefaultVisualEngine *              defaultVisualEngine;
    TVisualEngine *                     customVisualEngine;
    };

#endif /* _INCLUDE_tgsApplication_H_ */

