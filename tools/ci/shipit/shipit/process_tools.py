import asyncio
import asyncio.subprocess
import atexit
import subprocess
import logging
from typing import Union, List
import typing

class Result:
    def __init__(self, pid: int, exitcode: int, stdout: bytes, stderr: bytes, encoding: str) -> None:
        self.pid = pid
        self.exitcode = exitcode
        self.stdout = stdout
        self.stderr = stderr
        self.encoding = encoding

    @property
    def stdout_str(self) -> str:
        return self.stdout.decode(self.encoding)



async def _log_and_concat(logger: logging.Logger,
                    stream: asyncio.StreamReader,
                    encoding: str) -> bytes:
    output = b''
    while True:
        line_bytes = await stream.readline()
        if not line_bytes:
            break
        output += line_bytes  # Note we are not decoding this
        try:
            logger.debug("%s", line_bytes.decode(encoding).strip())  # but we are decoding here
        except UnicodeDecodeError as e:
            logger.error("Failed to decode program output: %s", e.reason) # TODO: fallback decode to hex or something?
    return output


# This function might look needlessly complicated but it's all required to be able to stream/pipe BOTH stdout and stderr
# simultaneously. Standard simple solutions such as read(), readline(), communicate(), etc are blocking.
# In such cases if you get no output on stderr you will be blocked and not get be able to read the output on stdout.
async def _run_logged_helper(command: Union[str, List[str]],
                       encoding: str,
                       shell: bool,
                       cwd: str = None) -> Result:
    if shell:
        command = typing.cast(str, command)
        s = await asyncio.create_subprocess_shell(command,
                                                  stdout=subprocess.PIPE,
                                                  stderr=subprocess.PIPE,
                                                  cwd=cwd,
                                                  limit=2**20)
    else:
        command = typing.cast(List[str], command)
        s = await asyncio.create_subprocess_exec(command[0], *command[1:],
                                                 stdout=subprocess.PIPE,
                                                 stderr=subprocess.PIPE,
                                                 cwd=cwd,
                                                 limit=2**20)
    logging.info("Executing command: %r, started PID=%d", command, s.pid)
    logger = logging.getLogger("PID%d" % s.pid)
    try:
        stdout, stderr = await asyncio.gather(
            _log_and_concat(logger, s.stdout, encoding),
            _log_and_concat(logger, s.stderr, encoding))
    except Exception:
        s.kill()
        raise
    finally:
        exitcode = await s.wait()
    return Result(s.pid, exitcode, stdout, stderr, encoding)


# Work around for problems with asyncio event loop termination on exit.
#
# The following is written to stderr upon exit if loop is not explicitly closed:
#
# Exception ignored in: <bound method BaseEventLoop.__del__ of <_UnixSelectorEventLoop running=False closed=True debug=False>>
# Traceback (most recent call last):
#  File "/usr/lib/python3.5/asyncio/base_events.py", line 510, in __del__
#  File "/usr/lib/python3.5/asyncio/unix_events.py", line 65, in close
#  File "/usr/lib/python3.5/asyncio/unix_events.py", line 146, in remove_signal_handler
#  File "/usr/lib/python3.5/signal.py", line 47, in signal
# TypeError: signal handler must be signal.SIG_IGN, signal.SIG_DFL, or a callable object
#
# There is a bug reported for this but it has been closed. See https://bugs.python.org/issue23548 .
# Unsure what the correct fix is. Creating a new event loop and closing it in each call to run()
# feels like the wrong approach (event loops are meant to be long lived... ?) and it also does not
# work (complains about resources not beeing "freed"). Regestering an exit handler that closes the
# event loop at least gets rid of the error message.
#
# TODO: Find a proper fix. Or wait until it is fixed in Python and remove this.
#       Tested on Python 3.5.3 - Error
#       Tested on Python 3.6 - FIXED (3.6 not available in Ubuntu 16.04 yet though)
def _close_event_loop():
    try:
        asyncio.get_event_loop().close()
    except:
        pass

atexit.register(_close_event_loop)


def run(command,
        encoding="utf-8",
        shell=False,
        timeout_sec: int = None,
        cwd: str = None) -> Result:
    loop = asyncio.get_event_loop()
    r = loop.run_until_complete(
        asyncio.wait_for(
            _run_logged_helper(command, encoding, shell, cwd=cwd),
            timeout_sec
        ))
    return r


def check_output_logged(command,
                        encoding="utf-8",
                        shell=False,
                        timeout_sec: int = None,
                        cwd: str = None) -> bytes:
    result = run(command, encoding=encoding, shell=shell, timeout_sec=timeout_sec, cwd=cwd)
    if result.exitcode != 0:
        logging.info("PID%d exit with non-0 exitcode: %d", result.pid, result.exitcode)
        raise subprocess.CalledProcessError(result.exitcode, command, result.stdout, result.stderr)
    logging.info("Process exit successfully")
    return result.stdout
