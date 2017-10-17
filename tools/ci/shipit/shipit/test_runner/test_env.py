import os
import logging
import tempfile
import stat
from shipit.process_tools import check_output_logged as run
logger = logging.getLogger(__name__)

vcc_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../../../../"))
aosp_root = os.path.abspath(os.path.join(vcc_root, "../../"))


def run_in_lunched_env(command: str, cwd: str):
    to_execute = """
source build/envsetup.sh
lunch ihu_vcc-eng
cd \"%s\"
%s""" % (os.path.abspath(cwd), command)
    logger.info("Executing command in shell: %s" % to_execute)

    with tempfile.NamedTemporaryFile("w") as f:
        f.write(to_execute)
        f.flush()
        os.chmod(f.name, os.stat(f.name).st_mode | stat.S_IXUSR)
        return run(["/bin/bash", f.name], cwd=aosp_root)

