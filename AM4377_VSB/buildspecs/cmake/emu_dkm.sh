#!/bin/sh
# emu_dkm.sh - sample CMAKE_CROSSCOMPILING_EMULATOR for VxWorks 7 DKMs
# Copyright (c) 2016 Wind River Systems, Inc. All Rights Reserved.
#
# USAGE:
# emu_dkm.sh LOGDIR TGTURI /path/to/kernel /path/to/prog.out [ARGS...]
#
# Executes the specified DKM "program.out" on the specified TGTURI,
# using the specified VxWorks Kernel. The DKM entry point must be 
# named "main". The TGTURI follows the syntax used in wrdbg, for
# example:
#    vxsim                     # Default simulator
#    vxworks7:10.0.0.1:1534    # vxworks7:IPAddress:Port
# All work of this script is logged in a file "emu_dkm.log" in the
# specified LOGDIR. More logging can be enabled if desired. That
# directory, as well as the directory of the dkm, must be writable.
#
# TODO - KNOWN LIMITATIONS:
# The method obtaining the program's exit code is only provisional.
# It requires the program under test to be compiled with debug
# info, and may fail with exit() nested deep in the program code.
# That said, it has been shown good enough for simple try_run
# executions as part of the cmake configure phase.
#
# modification history
# --------------------
# 18oct16,mob  written

mydir=`dirname $0`
LOGDIR=$1
TGTURI=$2
VXWORKS_KERNEL=$3
PROG=$4
MASTERLOG=${LOGDIR}/emu_dkm.log

# Error handling and SHIFT to get rest of the arguments
if [ ! -d "${LOGDIR}" ]; then
  echo "Error: Passed LOGDIR is not a directory: ${LOGDIR}"
  exit 128
fi
echo "---emu_dkm.sh---" >> "${MASTERLOG}"
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
QARGS=""
while [ "$1" != "" ]; do
  case x$1 in
    x[0-9]*) QARGS="${QARGS} $1" ;;
    *)       QARGS="${QARGS} \"$1\"" ;;
  esac
  shift
done
echo "QARGS=${QARGS}" >> "${MASTERLOG}"

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
SCRIPT=${TMPLOGD}/$(basename "$PROG").wrdbg
TMPOUT=${SCRIPT}.output
TMPLOG=${SCRIPT}.log

# Generate a command file for wrdbg, next to the program to execute...
# single-step until a source line hints us to the exit code.
# TODO improve this once wrdbg is able to return the exit code.
# TODO wrdbg doesn't like dbl quotes for the Kernel or logfile - see WB4-7073
#echo "target connect $TGTURI -kernel \"${VXWORKS_KERNEL}\" -logfile \"${TMPLOG}\"" > "${SCRIPT}"
echo "target connect $TGTURI -kernel ${VXWORKS_KERNEL} -logfile ${TMPLOG}" > "${SCRIPT}"
echo "show quiet" >> "${SCRIPT}"
echo "set quiet off" >> "${SCRIPT}"
echo "module load \"$PROG\"" >> "${SCRIPT}"
echo "task create main ${QARGS}" >> "${SCRIPT}"
#echo break main >> "${SCRIPT}"
#echo continue >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo next >> "${SCRIPT}"
echo continue >> "${SCRIPT}"
echo target disconnect >> "${SCRIPT}"
echo quit >> "${SCRIPT}"

echo wrdbg --command "${SCRIPT}" >> "${MASTERLOG}"
wrdbg --command "${SCRIPT}" | tee "${TMPOUT}"
rc=$?
cat "$TMPOUT" >> "${MASTERLOG}"
if [ $rc != 0 ]; then
  echo "ERRORLEVEL $rc" >> "${MASTERLOG}"
  echo "LOGDIR: ${TMPLOGD}" >> "${MASTERLOG}"
  exit $rc
fi
echo DONE-OK >> "${MASTERLOG}"

# Since we have no I/O redirection for DKMs, we just print the logfile to stdout.
# TODO: This works only for vxsim -- connections to real targets won't log output.
if [ -f "${TMPLOG}" ]; then
  cat "${TMPLOG}" >> "${MASTERLOG}"
  cat "${TMPLOG}"
else
  echo "------------------------------------------------------------------------------"
  echo "WARNING: No Output available from launched tasks! - Only exitcode is returned!"
  echo "WARNING: No Output available from launched tasks! - Only exitcode is returned!" >> "${MASTERLOG}"
  echo "------------------------------------------------------------------------------"
fi

# Try to obtain the exit code.
# TODO this should be replaced by a more reliable method eg using wrpython.
grep '^[1-9].*[^a-zA-Z_]\(return\|exit\)[^a-zA-Z_].*[0-9].*;' "${TMPOUT}" > "${SCRIPT}.exitcode"
rc=$?
cat "${SCRIPT}.exitcode" >> "${MASTERLOG}"
if [ $rc = 0 ]; then
  rc=`tail -1 "${SCRIPT}.exitcode" | sed -e 's,^.*e.*t[^0-9]*\([0-9][0-9]*\).*$,\1,'`
  echo "EXITCODE $rc" >> "${MASTERLOG}"
  rm -rf "${TMPLOGD}"
  exit $rc
fi

grep "^[1-9].*return.*1;" "${TMPOUT}" >> "${MASTERLOG}"
rc=$?
if [ $rc = 0 ]; then
  echo "EXITCODE 1" >> ${MASTERLOG}
  rm -rf "${TMPLOGD}"
  exit 1
fi
grep "^[1-9].*exit.*0" "${TMPOUT}" >> "${MASTERLOG}"
rc=$?
if [ $rc = 0 ]; then
  echo "EXITCODE 0" >> "${MASTERLOG}"
  rm -rf "${TMPLOGD}"
  exit 0
fi
grep "^[1-9].*exit.*1" "${TMPOUT}" >> "${MASTERLOG}"
rc=$?
if [ $rc = 0 ]; then
  echo EXITCODE 1 >> ${MASTERLOG}
  rm -rf "${TMPLOGD}"
  exit 1
fi
echo ------- UNKNOWN EXITCODE -------- >> ${MASTERLOG}
echo "--- ERROR: UNKNOWN EXITCODE ---"
echo "The program probably failed to load. For details, see the logdir:"
echo "${TMPLOGD}"
echo "-------------------------------"
#tail -20 "${MASTERLOG}"

exit 127
