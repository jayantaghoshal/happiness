import asyncio
import asyncio.subprocess
import subprocess
import logging
from typing import Union, List


class Result:
    def __init__(self, pid: int, exitcode: int, stdout: bytes, stderr: bytes, encoding: str):
        self.pid = pid
        self.exitcode = exitcode
        self.stdout = stdout
        self.stderr = stderr
        self.encoding = encoding

    @property
    def stdout_str(self) -> str:
        return self.stdout.decode(self.encoding)


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
def _run_logged_helper(command: Union[str, List[str]],
                       encoding: str,
                       shell: bool,
                       *args,
                       **kwargs) -> Result:
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
    return Result(s.pid, exitcode, stdout, stderr, encoding)


def run(command,
        encoding="utf-8",
        shell=False,
        timeout_sec: int = None,
        *args,
        **kwargs) -> Result:
    loop = asyncio.get_event_loop() # Something fishy about this, it is not closed properly on shut down..
    return loop.run_until_complete(
        asyncio.wait_for(
            _run_logged_helper(command, encoding, shell, *args, **kwargs),
            timeout_sec
        ))


def check_output_logged(command,
                        encoding="utf-8",
                        shell=False,
                        timeout_sec: int = None,
                        *args,
                        **kwargs) -> bytes:
    result = run(command, encoding=encoding, shell=shell, timeout_sec=timeout_sec, *args, **kwargs)
    if result.exitcode != 0:
        logging.info("PID%d exit with non-0 exitcode: %d", result.pid, result.exitcode)
        raise subprocess.CalledProcessError(result.exitcode, command, result.stdout, result.stderr)
    logging.info("Process exit successfully")
    return result.stdout
