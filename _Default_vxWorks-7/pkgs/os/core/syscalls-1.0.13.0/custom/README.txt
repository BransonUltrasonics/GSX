#############################################################################
# Purpose
#############################################################################

This layer holds the default custom system calls. It does not actually add 
anything to the VxWorks system. It is intended to be used as a template for
a user-provided custom system call layer which will have actual content and
will supersede this default layer.

#############################################################################
# Layer's Organization
#############################################################################

This layer is organized as follows:

   custom
    |
    +- kernel_h   <- holds the headers for type definitions needed
    |                by the system call handlers (optional)
    |
    +- kernel_src <- holds the source of the system call handlers
    |                and makefile
    |
    +- pre_src    <- holds the (system) makefile calling scgen.  DO NOT CHANGE
    |
    +- syscall_defs <- holds the custom system call definition files 
    |
    +- user_h     <- holds the headers providing the prototypes of the custom
                     system calls (optional)

Some of the items in the custom system call layer should not be modified by
customers:

  - the top-level Makefile in the layer
  - the name and content of the "pre_src" directory

Some of the items may be modified following strict rules:

  - the content of the "syscall_defs" directory.

The content of the other directories may be changed more freely although
rules do apply to them too.

#############################################################################
# Adding Custom System Calls
#############################################################################

Adding custom system calls entails the following steps:

  1. add the API and number for the custom system calls in the custom system
     call definition files.

  2. add the source file(s) for the custom system call handlers, as well as the
     Makefile for compiling them

  3. if necessary add the kernel-side header file(s) holding the type and macro
     definitions needed by the custom system call handlers.

  4. if so desired add the RTP-side header file(s) holding the custom system
     calls' prototypes and any useful type and macro definitions.

The custom system call's API and number definitions go in syscallUsrApi.def
and syscallUsrNum.def files located in the "syscall_defs" directory. Sample
files already exist in that directory and can be used as a starting point.
Note that they must be renamed from .def.sample into .def files. The required
syntax for those files is described in the VxWorks Kernel Programmer's Guide.

The source file(s) for the custom system call handlers go in the "kernel_src"
directory. Sample files already exist in that directory and can be used as a
starting point. How to develop system call handlers is described in the
VxWorks Kernel Programmer's Guide.

Kernel-side header file(s) holding type and macro definitions needed by the
custom system call handlers go in the "kernel_h" directory. A sample file is
provided there too.

Finally, RTP-side header file(s) holding the custom system calls' prototypes
go in the "user_h" directory. Again, a sample file is provided.

#############################################################################
# Creating and Using a Variant of the Layer
#############################################################################

Customers can create and use a variant of the custom system call layer via the
following set of steps:

  1. copy of the entire default layer to some other location, say
     /folk/pad/myProject:

        cd $WIND_BASE/pkgs/os/core/syscalls
        cp -R custom /folk/pad/myProject
          
  2. update the new layer's version number: VERSION attribute in layer.vsbl.
     Customer's may not update the new layer's name (otherwise it will not
     replace the default custom system call layer) so, in order to avoid
     version collision with Wind River's default custom system call layer
     in the future, customers must start with the version 2.0.0.0.

  3. if needed update the new layer's parent information: PARENT attribute in
     layer.vsbl. Note that the PARENT attribute is required since the copy of
     the layer is meant to be located outside of the product's installation.
     The potential need to update this attribute is due to the fact that the
     parent layer's version is indicated in that attribute. For instance:
     SYSCALLS_1_0_0_0 may have to become SYSCALLS_2_0_0_0 if the SYSCALLS
     layer's version has changed between when the default custom system call
     layer had been copied and when its copy is used.

  4. update the custom system call definition files to define the new
     system call's API, number and group: this is done by editing the new
     layer's syscallUsrNum.def and syscallUsrApi.def files.

  5. add all the user-provided content (see previous section) in the
     appropriate directories of the new layer.

  6. set the WIND_LAYER_PATHS environment variable in a development shell
     session so that it points to the directory holding the variant of the
     layer (in the example, /folk/pad/myProject/).

  7. create a VSB project and add the new layer to it (if the new layer has
     the same name as the default one it will automatically be used instead of
     the default layer).

  8. build the VSB. This will result in system libraries supporting the custom
     system calls and an RTP development environment providing definitions for
     those custom system calls.

#############################################################################
# Sample source file of an RTP application using the check_vxworks_version()
# custom system call.
#############################################################################

/* checkVxWorksVersion.c - demonstrate static custom system calls */

/* Copyright 2011 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,31jan11,pad   Written
*/

/*
SYNOPSIS

\cs
checkVxWorksVersion.vxe
\ce

DESCRIPTION
This small application demonstrates the us of a static custom system call. This system call, check_vxworks_version(), must have been already added to the
VxWorks image.

\NOROUTINES
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <mySystemCall.h>

/******************************************************************************
*
* main - main routine of the RTP
*
* This program uses the static custom system call check_vxworks_version() to
* get the version information of the VxWorks system on which it executes.
*
* Here is an ouput example:
*
* This program executed on VxWorks 6.9.0
* VxWorks image created Jan 31 2011, 15:53:52
*
*/

void main (void)
    {
    char creationDate [30];
    vxWorksVersionInfo vxVersionInfo;

    if (check_vxworks_version  (sizeof creationDate, creationDate,
				&vxVersionInfo) != OK)
	{
	printf ("Error while getting VxWorks version information: "
		"errno = %#x\n", errno);
	exit (1);
	}

    printf ("This program executed on VxWorks %d.%d.%d\n", vxVersionInfo.major,
	    vxVersionInfo.minor, vxVersionInfo.maint);
    printf ("VxWorks image created %s\n", creationDate);

    exit (0);
    }

#############################################################################
# Content of RTP application makefile for generating the RTP executable
#############################################################################

EXE = checkVxWorksVersion.vxe

RTP_BASE_DIR = checkVxWorksVersion

OBJS =  checkVxWorksVersion.o

include $(WIND_USR_MK)/rules.rtp.mk

