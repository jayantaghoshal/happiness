import asyncio
import asyncio.subprocess
import subprocess
import logging
from typing import Union, List


@asyncio.coroutine
def _log_and_concat(logger: logging.Logger,
                    stream: asyncio.StreamReader,
                    encoding: str):
    output = []
    while True:
        line_bytes = yield from stream.readline()
        if not line_bytes:
            break
        output.append(line_bytes)  # Note we are not decoding this
        try:
            logger.debug("%s", line_bytes.decode(encoding).strip())
        except UnicodeDecodeError as e:
            logger.error("Failed to decode program output: %s", e.reason)
    return b''.join(output)


# This function might look needlessly complicated but it's all required to be able to stream/pipe BOTH stdout and stderr
# simultaneously. Standard simple solutions such as read(), readline(), communicate(), etc are blocking.
# In such cases if you get no output on stderr you will be blocked and not get be able to read the output on stdout.
@asyncio.coroutine
def _check_output_logged_helper(command: Union[str, List[str]],
                                encoding: str,
                                shell: bool,
                                *args,
                                **kwargs):
    if shell:
        s = yield from asyncio.create_subprocess_shell(command,
                                                       stdout=subprocess.PIPE,
                                                       stderr=subprocess.PIPE,
                                                       *args, **kwargs)
    else:
        s = yield from asyncio.create_subprocess_exec(command[0], *command[1:],
                                                      stdout=subprocess.PIPE,
                                                      stderr=subprocess.PIPE,
                                                      *args, **kwargs)
    logging.info("Executing command: %r, started PID=%d", command, s.pid)
    logger = logging.getLogger("PID%d" % s.pid)
    try:
        stdout, stderr = yield from asyncio.gather(
            _log_and_concat(logger, s.stdout, encoding),
            _log_and_concat(logger, s.stderr, encoding))
    except Exception:
        s.kill()
        raise
    finally:
        exitcode = yield from s.wait()
    return s.pid, exitcode, stdout, stderr


def check_output_logged(command,
                        encoding="utf-8",
                        shell=False,
                        timeout_sec: int = None,
                        *args,
                        **kwargs):
    loop = asyncio.get_event_loop() # Something fishy about this, it is not closed properly on shut down..
    pid, exitcode, stdout, stderr = loop.run_until_complete(
        asyncio.wait_for(
            _check_output_logged_helper(command, encoding, shell, *args, **kwargs),
            timeout_sec
        ))
    if exitcode != 0:
        logging.info("PID%d exit with non-0 exitcode: %d", pid, exitcode)
        raise subprocess.CalledProcessError(exitcode, command, stdout, stderr)
    logging.info("Process exit successfully")
    return stdout
