# runrtp.py - WindRiver gdb interface example to start an RTP for vxworks7.

# Copyright (c) 2016 Wind River Systems, Inc.
#
# The right to copy, distribute, modify, or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.

import argparse
import gdb
import os
import re
import sys
import time

import windriver.shells.wrdbg.utils.utils as wrdbgutils
import windriver.tcf.utils.utils as wrtcfutils
import windriver.utils.utils as wrutils


class ProcessesListener(object):
    """See ``opensource.tcf.services.processes`` for more info."""

    exitcodes = {}
    exitcode = None

    def exited(self, process_id, exit_code):
        """Called when a process exits."""
        self.exitcode = exit_code
        self.exitcodes[process_id] = exit_code

# ----------------------------- Parse arguments ----------------------------- #

parser = argparse.ArgumentParser(description='RTP run code sample',)

parser.add_argument('-debug', action='store_true', help='Turn on debug logs.')
parser.add_argument('-kernel', required=True, help='Target kernel path.')
parser.add_argument('-logdir', help='Log directory if any.')
parser.add_argument('targetid',
                    help='TCF peer ID to connect to. Might be "vxsim".')
parser.add_argument('rtp', help='RTP to run.')
parser.add_argument('rtpargs', nargs=argparse.REMAINDER,
                    help='Arguments for the RTP (if any).')

logdir = None
parsed = parser.parse_args(sys.argv[1:])

kernel = wrutils.fullpath(parsed.kernel)
rtp = wrutils.fullpath(parsed.rtp)
if parsed.logdir:
    logdir = wrutils.fullpath(parsed.logdir)
    # Create log dir if needed.
    if not os.path.exists(logdir):
        try:
            os.makedirs(logdir)
        except Exception as e:
            msg = '*** WARNING ***\nCould not create log directory "' + \
                  str(logdir) + '" : ' + str(e) + '.\nLog is turned off.\n'
            sys.stderr.write(msg)
            logdir = None

try:
    # Connect to the target

    if logdir and parsed.targetid == 'vxsim':
        # VxSim log is available only if we start the vxsim.
        logfilepath = wrutils.fullpath((logdir, 'vxsim-log.txt'))
        logfile = ' -logfile ' + logfilepath
        # For Windows issues, the log file must be removed first.
        if os.path.exists(logfilepath):
            os.remove(logfilepath)
    else:
        logfile = ''

    if parsed.debug and logdir:
        debuglog = wrutils.fullpath((logdir, 'wrdbg-debug.txt'))
        gdb.execute('set debug filename ' + debuglog)
        gdb.execute('set debug save on')
        # Add the TCF launcher logs too.
        logfile += ' -logdir ' + str(logdir) + ' -logverbosity 2'

    gdb.execute('target connect ' + parsed.targetid + ' -kernel ' + kernel +
                logfile)

    # Now that we are connected, we can instantiate the process listener here,
    # and add it to the process service.

    prcsvc = wrtcfutils.getService('Processes')
    listener = ProcessesListener()
    prcsvc.addListener(listener)

    gdb.execute('set quiet off')

    # Create RTP arguments. Do not forget to surround with double quotes if
    # needed

    rtpargs = ''
    for rtparg in parsed.rtpargs:
        if ' ' in rtparg:
            rtpargs += ' "' + rtparg + '"'
        else:
            rtpargs += ' ' + rtparg

    created = gdb.execute('rtp create ' + rtp + rtpargs, False, True)

    # Get the TCF ID of the started RTP. Remove if not useful.

    match = re.search('RTP created: id = (?P<rtpid>\w+),', created)
    tcfid = None
    if match:
        rtpid = match.group('rtpid')
        tcfid = wrdbgutils.targetIDToTcfID(rtpid)
        if tcfid == rtpid:
            # Seems like targetIDToTcfID() failed.
            tcfid = None

    # In order for this script to handle the CTRL+C once the RTP is resumed,
    # better run it in background.

    gdb.execute('continue &')

    # Here we should be able to get the RTP exit code.

    if tcfid:
        # We can make sure this is the RTP we started.
        while tcfid not in listener.exitcodes:
            time.sleep(1)
    else:
        while listener.exitcode is None:
            time.sleep(1)

    sys.stdout.write('RTP exited with exit code : ' + str(listener.exitcode) +
                     '\n')

except KeyboardInterrupt as ke:
    sys.stderr.write('Interrupted by user ...\n')
except Exception as e:
    sys.stderr.write('*** ERROR ***\n' + str(e) + '\n')
finally:
    gdb.execute('target disconnect')

quit()
