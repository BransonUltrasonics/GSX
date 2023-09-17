#!/bin/sh
# emu_rtp.sh - sample CMAKE_CROSSCOMPILING_EMULATOR for VxWorks 7 RTPs
# Copyright (c) 2016 Wind River Systems, Inc. All Rights Reserved.
#
# USAGE:
# emu_rtp.sh LOGDIR TGTURI /path/to/kernel /path/to/prog.vxe [ARGS...]
#
# Executes the specified RTP "program.vxe" on the specified TGTURI,
# using the specified VxWorks Kernel. The TGTURI follows the syntax
# used in wrdbg, for example:
#    vxsim                     # Default simulator
#    vxworks7:10.0.0.1:1534    # vxworks7:IPAddress:Port
# All work of this script is logged in a file "emu_rtp.log" in the
# specified LOGDIR. More logging can be enabled if desired. That
# directory, as well as the directory of the vxe, must be writable.
#
# modification history
# --------------------
# 18oct16,mob  written

mydir=`dirname $0`
LOGDIR=$1
TGTURI=$2
VXWORKS_KERNEL=$3
PROG=$4
MASTERLOG=${LOGDIR}/emu_rtp.log

# Error handling and SHIFT to get rest of the arguments
if [ ! -d "${LOGDIR}" ]; then
  echo "Error: Passed LOGDIR is not a directory: ${LOGDIR}"
  exit 128
fi
echo "---emu_rtp.sh---" >> "${MASTERLOG}"
echo "$@" >> "${MASTERLOG}"
if [ ! -f "${VXWORKS_KERNEL}" ]; then
  ERR="Error: Passed VXWORKS_KERNEL not found: ${VXWORKS_KERNEL}"
  echo "$ERR"
  echo "$ERR" >> "${MASTERLOG}"
  exit 128
elif [ ! -f "${PROG}" ]; then
  ERR="Error: Passed PROG not found: ${PROG}"
  echo "$ERR"
  echo "$ERR" >> "${MASTERLOG}"
  exit 128
fi
shift 4

# Environment variables can override what is set from cmake.
if [ "$VXWORKS_TEST_KERNEL" != "" ]; then
  echo "VXWORKS_KERNEL overridden by environment: $VXWORKS_TEST_KERNEL"
  VXWORKS_KERNEL=$VXWORKS_TEST_KERNEL
fi
if [ "$VXWORKS_TEST_TARGET" != "" ]; then
  echo "TGTURI overridden by environment: $VXWORKS_TEST_TARGET"
  TGTURI=$VXWORKS_TEST_TARGET
fi

# Create temporary logdir to hold logs, in order to enable multiple test runs in parallel.
TMPLOGD=${LOGDIR}/wrdbg.$$
while [ -d "${TMPLOGD}.tmp" ]; do
  TMPLOGD="${TMPLOGD}_"
done
# TODO there is a short race condition here - someone else could create
# The directory after we determined it is not there but before we create it
TMPLOGD=${TMPLOGD}.tmp
mkdir -p "${TMPLOGD}"
TMPOUT=${TMPLOGD}/$(basename "$PROG").wrdbg.output

# Use wrpython to run a debugger script to reliably get the exitcode.
# TODO instead of our own emu_runrtp.py, use the official tested copy of this:
# TODO WIND_HOME\workbench-4\x86-win32\lib\python\tests\samples\runrtp.py 
wrpython $mydir/emu_runrtp.py $TGTURI \
    -kernel "$VXWORKS_KERNEL" \
    -logdir "${TMPLOGD}" "$PROG" "$@" | tee "${TMPOUT}"
rc=$?
cat "$TMPOUT" >> "${MASTERLOG}"
if [ $rc != 0 ]; then
  echo "ERRORLEVEL $rc" >> "${MASTERLOG}"
  echo "LOGDIR: ${TMPLOGD}" >> "${MASTERLOG}"
  exit $rc
fi
echo DONE-OK >> "${MASTERLOG}"

# Try to obtain the exit code.
grep "RTP exited with exit code : " "$TMPOUT" > "${TMPLOGD}/wrdbg.exitcode"
rc=$?
if [ $rc = 0 ]; then
  rc=`sed -e 's,[^0-9]*,,g' "${TMPLOGD}/wrdbg.exitcode"`
  echo EXITCODE $rc >> "${MASTERLOG}"
  rm -rf "${TMPLOGD}"
  exit $rc
fi
echo ------- UNKNOWN EXITCODE -------- >> ${MASTERLOG}
echo "--- ERROR: UNKNOWN EXITCODE ---"
echo "The program probably failed to load. For details, see the logdir:"
echo "${TMPLOGD}"
echo "-------------------------------"
#tail -20 "${MASTERLOG}"

exit 127
