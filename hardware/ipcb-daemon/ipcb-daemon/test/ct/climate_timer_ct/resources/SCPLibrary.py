import os
import paramiko

class SCPLibrary(object):
    def __init__(self):
        self.ssh = None
        self.sftp = None

    def open_connection(self, hostname, port=22, username=None, password=None):
        self.ssh = paramiko.SSHClient()
        self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.ssh.connect(hostname, port=port, username=username, password=password)
        self.sftp = self.ssh.open_sftp()

    def close_connection(self):
        self.ssh.close()
        self.sftp.close()

    def get_file(self, remote_file, local_file):
        self.sftp.get(remote_file, local_file)

    def put_file(self, local_file, remote_file):
        self.sftp.put(local_file, remote_file)

