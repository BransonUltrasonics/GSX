/* tgsPath.h - Tilcon path class */

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
14mar16,jnl support window surface (F6570)
29jul15,c_l support circle,ellipse and arc (V7GFX-247)
01jan14,yya created
*/

/*
DESCRIPTION

This file defines the Tilcon's path class. TPath is used to describe the path
primitive in vector graphics.

INCLUDE FILES: tilcon/engine/tgsPath.h
*/

#ifndef _INCLUDE_tgsPath_H_
#define _INCLUDE_tgsPath_H_

#include "tilcon/engine/tgsFont.h"

class TPath
    {
    friend class TVectorPainter;
public:
/*******************************************************************************
*
* TPath - construct a empty path
*
* SYNOPSIS
* \ss
* TPath()
* \se
*
* DESCRIPTION
* This function constructs a TPath instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath();

/*******************************************************************************
*
* TPath - construct a path by copying from another path
*
* SYNOPSIS
* \ss
* TPath
*     (
*     const TPath & path    /@ path to copy from @/
*     )
* \se
*
* This function constructs a path by copying another TPath instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath(const TPath &);

/*******************************************************************************
*
* TPath - construct a path with array of points
*
* SYNOPSIS
* \ss
* TPath
*     (
*     TRT_U32 n,                /@ number of commands @/
*     const TPointF * pts       /@ point array @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPath instance with array of points. The specified
* points are connected to together by "line to" command to form the TPath.
*
* Parameters:
* \is
* \i <n>
* the number of points to be specified;
* \i <pts>
* the point array;
* \ie
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath
        (
        TRT_U32 n,
        const TPointF * pts
        );

/*******************************************************************************
*
* TPath - construct a path with array of commands and coordinates
*
* SYNOPSIS
* \ss
* TPath
*     (
*     TRT_U32 n,                /@ number of commands @/
*     const TRT_BYTE * cmds,    /@ command array @/
*     const TRT_FLOAT * coords  /@ coordinates array @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPath instance and appends the specified vector
* data to the constructed path.
*
* Parameters:
* \is
* \i <n>
* the number of commands in the command list to be specified;
* \i <cmds>
* the address of the command list; it shall be an array of TRT_BYTE
* (8-bit integer), and the length of the array shall be <n>;
* \i <coords>
* the address of the coordinates array; it shall be an array of TRT_FLOAT type;
* each command may need 0~6 coordinates, so the length of coordinates array is
* decided by the types of the commands specified in <cmds>.
* \ie
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath
        (
        TRT_U32 n,
        const TRT_BYTE * cmds,
        const TRT_FLOAT * coords
        );

/*******************************************************************************
*
* TPath - construct a closing path from rectangle
*
* SYNOPSIS
* \ss
* TPath
*     (
*     const TRect & rect   /@ source rectangle @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a closing path from the provided rectangle.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath(const TRect & rect);

/*******************************************************************************
*
* TPath - construct a path from a text string
*
* SYNOPSIS
* \ss
* TPath
*     (
*     const TFont & font,       /@ font of the text @/
*     const TWString & string   /@ content of the text @/
*     )
* \se
*
* DESCRIPTION
* This function constructs a TPath using a text string with a specific font.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath(const TFont & font, const TWString & string);

/*******************************************************************************
*
* ~TPath - destruct the path
*
* This function destructs the TPath instance.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* NOMANUAL
*
* SEE ALSO:
*/
    ~TPath();

/*******************************************************************************
*
* operator = - copy path
*
* SYNOPSIS
* \ss
* TPath & operator =
*     (
*     const TPath & path    /@ path to copy from @/
*     )
* \se
*
* This function copies from another TPath instance.
*
* RETURNS: reference to self
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TPath & operator = (const TPath &);

/*******************************************************************************
*
* Append - append another path to this path
*
* SYNOPSIS
* \ss
* void Append
*     (
*     const TPath & path                    /@ path to append @/
*     )
* \se
*
* DESCRIPTION
* This function appends another path to this path.
* This function throws std::bad_alloc on memory allocation failure.
*
* Parameters:
* \is
* \i <path>
* the path to append to this path;
* \ie
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: AppendData()
*/
    void Append(const TPath & path);

/*******************************************************************************
*
* AppendData - append data to this path
*
* SYNOPSIS
* \ss
* void AppendData
*     (
*     TRT_U32 n,                /@ number of path commands to append @/
*     const TRT_BYTE * cmds,    /@ array of commands to append @/
*     const TRT_FLOAT * coords  /@ array of coordinates to append @/
*     )
* \se
*
* DESCRIPTION
* This function appends data to this path.
* This function throws std::bad_alloc on memory allocation failure.
*
* Parameters:
* \is
* \i <n>
* the number of commands to append;
* \i <cmds>
* the array of commands to append;
* \i <coords>
* the array of coordinates to append;
* \ie
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: Append()
*/

    void AppendData(TRT_U32 n, const TRT_BYTE *, const TRT_FLOAT *);

/*******************************************************************************
*
* MoveTo - add a Move-To segment to the path
*
* SYNOPSIS
* \ss
* void MoveTo
*     (
*     TRT_FLOAT x, /@ x coordinate of the Move To command @/
*     TRT_FLOAT y  /@ y coordinate of the Move To command @/
*     )
* \se
*
* DESCRIPTION
* This function adds a Move-To segment to the path, with the specified
* destination coordinate.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: LineTo(), QuadTo(), CubicTo(), AddText(), Close(), Clear()
*/
    void MoveTo
        (
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* LineTo - add a Line-To segment to the path
*
* SYNOPSIS
* \ss
* void LineTo
*     (
*     TRT_FLOAT x, /@ x coordinate of the Line To command @/
*     TRT_FLOAT y  /@ y coordinate of the Line To command @/
*     )
* \se
*
* DESCRIPTION
* This function adds a Line-To segment to the path, with the specified
* destination coordinate.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), QuadTo(), CubicTo(), AddText(), Close(), Clear()
*/
    void LineTo
        (
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* QuadTo - add a Quad-To segment to the path
*
* SYNOPSIS
* \ss
* void QuadTo
*     (
*     TRT_FLOAT cx, /@ x of the control coordinate @/
*     TRT_FLOAT cy, /@ y of the control coordinate @/
*     TRT_FLOAT x, /@ x of the destination coordinate @/
*     TRT_FLOAT y  /@ y of destination coordinate @/
*     )
* \se
*
* DESCRIPTION
* This function adds a Quad-To segment to the path, with the specified
* control coordinate and destination coordinate.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), LineTo(), CubicTo(), AddText(), Close(), Clear()
*/
    void QuadTo
        (
        TRT_FLOAT cx,
        TRT_FLOAT cy,
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* CubicTo - add a Cubic-To segment to the path
*
* SYNOPSIS
* \ss
* void CubicTo
*     (
*     TRT_FLOAT cx1,    /@ x of the first control coordinate @/
*     TRT_FLOAT cy1,    /@ y of the first control coordinate @/
*     TRT_FLOAT cx2,    /@ x of the second control coordinate @/
*     TRT_FLOAT cy2,    /@ y of the second control coordinate @/
*     TRT_FLOAT x,      /@ x of the destination coordinate @/
*     TRT_FLOAT y       /@ y of destination coordinate @/
*     )
* \se
*
* DESCRIPTION
* This function adds a Cubic-To segment to the path, with the specified
* control coordinates and destination coordinates.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), LineTo(), QuadTo(), AddText(), Close(), Clear()
*/
    void CubicTo
        (
        TRT_FLOAT cx1,
        TRT_FLOAT cy1,
        TRT_FLOAT cx2,
        TRT_FLOAT cy2,
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* CubicTo - add a Cubic-To segment to the path
*
* SYNOPSIS
* \ss
* void CubicTo
*     (
*     const TPoint & cp1,   /@ the first control coordinate @/
*     const TPoint & cp2,   /@ the second control coordinate @/
*     const TPoint & p      /@ the destination coordinate @/
*     )
* \se
*
* DESCRIPTION
* Overloads of the CubicTo function.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), LineTo(), QuadTo(), AddText(), Close(), Clear()
*/
    void CubicTo
        (
        const TPointF & cp1,
        const TPointF & cp2,
        const TPointF & p
        )
        {
        CubicTo(cp1.x, cp1.y, cp2.x, cp2.y, p.x, p.y);
        }

/*******************************************************************************
*
* AddText - add a path from a string
*
* SYNOPSIS
* \ss
* void AddText
*     (
*     const TFont & font,       /@ text font @/
*     const TWString & text,    /@ text content @/
*     )
* \se
*
* DESCRIPTION
* This function add a path to this path. The added path is created from the
* specified font and specified string.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), LineTo(), QuadTo(), CubicTo(), Close(), Clear()
*/
    void AddText(const TFont & font, const TWString & text);

/*******************************************************************************
*
* Close - add a Close segment to the path
*
* SYNOPSIS
* \ss
* void Close()
* \se
*
* DESCRIPTION
* This function adds a Close segment to the path; The close segment is
* effectively a Line-To segment from the current destination to the start of
* the path. Closed path has a closed interior region which can be filled.
* This function throws std::bad_alloc on memory allocation failure.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), LineTo(), QuadTo(), CubicTo(), Clear()
*/
    void Close();

/*******************************************************************************
*
* Clear - remove all segments in the path
*
* SYNOPSIS
* \ss
* void Clear()
* \se
*
* DESCRIPTION
* This function removes all segments in the path, making it an empty path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: MoveTo(), LineTo(), QuadTo(), CubicTo(), Close()
*/
    void Clear();

/*******************************************************************************
*
* Translate - translate the path with offsets in X and Y direction
*
* SYNOPSIS
* \ss
* TPath & Translate
*     (
*     TRT_FLOAT x,  /@ translate applied to x axis @/
*     TRT_FLOAT y   /@ translate applied to y axis @/
*     )
* \se
*
* DESCRIPTION
* This function translate the path with offsets in X and Y direction.
*
* RETURNS: reference to the path itself.
*
* ERRNO: N/A
*
* SEE ALSO: Scale(), Rotate(), Transform()
*/
    TPath & Translate
        (
        TRT_FLOAT x,
        TRT_FLOAT y
        );

/*******************************************************************************
*
* Scale - scale the path with coefficient in X and Y direction
*
* SYNOPSIS
* \ss
* TPath & Scale
*     (
*     TRT_FLOAT sx, /@ scale applied to x axis @/
*     TRT_FLOAT sy  /@ scale applied to y axis @/
*     )
* \se
*
* DESCRIPTION
* This function scales the path with coefficient in X and Y direction.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: Translate(), Rotate(), Transform()
*/
    TPath & Scale
        (
        TRT_FLOAT sx,
        TRT_FLOAT sy
        );

/*******************************************************************************
*
* Rotate - rotate the path with specified degree
*
* SYNOPSIS
* \ss
* TPath & Rotate
*     (
*     TRT_FLOAT d   /@ degree to rotate @/
*     )
* \se
*
* DESCRIPTION
* This function rotates the path with specified degree.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: Translate(), Scale(), Transform()
*/
    TPath & Rotate
        (
        TRT_FLOAT d
        );

/*******************************************************************************
*
* Transform - apply a transformation matrix to the path
*
* SYNOPSIS
* \ss
* TPath & Transform
*     (
*     const TMatrix3x3 & m  /@ transformation matrix @/
*     )
* \se
*
* DESCRIPTION
* This function applies a transformation matrix to the path.
*
* RETURNS: N/A.
*
* ERRNO: N/A
*
* SEE ALSO: Translate(), Scale(), Rotate()
*/
    TPath & Transform
        (
        const TMatrix3x3 & m
        );

/*******************************************************************************
*
* GetBoundRect - get the bounding rectangle the path
*
* SYNOPSIS
* \ss
* TRectF GetBoundRect() const
* \se
*
* DESCRIPTION
* This function gets the bounding rectangle the path.
*
* RETURNS: the bounding rectangle the path.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRectF GetBoundRect() const;

/*******************************************************************************
*
* GetHandle - get the platform specific object handle
*
* SYNOPSIS
* \ss
* TRT_HANDLE GetHandle() const
* \se
*
* DESCRIPTION
* This function returns the object handle.
*
* RETURNS: object handle.
*
* ERRNO: N/A
*
* SEE ALSO:
*/
    TRT_HANDLE GetHandle() const
        {
        return data->handle;
        }

private:
    class Data
        {
    public:
        Data(TRT_PATH_HANDLE h);
        ~Data();
        TRT_PATH_HANDLE  handle;
        };

    TSharedPtr<Data>   data;
    };

#endif /* _INCLUDE_tgsPath_H_ */

