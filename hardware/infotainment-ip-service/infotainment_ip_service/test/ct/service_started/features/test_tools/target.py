from subprocess import check_output
from platform import _release_filename

def execute_on_target(cmd):
    result = check_output(["sshpass", "-p", "root",
                           "ssh",
                           "-o", "StrictHostKeyChecking=no",
                           "-o", "UserKnownHostsFile=/dev/null",
                           "-o", "LogLevel=ERROR",
                           "-o", "ConnectTimeout=30",
                           "root@198.18.34.1 ",
                           cmd])
    return result

def copy_file_from_target(source, destination):
    result = check_output(["sshpass", "-p", "root",
                           "scp",
                           "-o", "StrictHostKeyChecking=no",
                           "-o", "UserKnownHostsFile=/dev/null",
                           "-o", "LogLevel=ERROR",
                           "-o", "ConnectTimeout=30",
                           "root@198.18.34.1:" + source,
                           destination])
    return result
