# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import json
import os
import subprocess
import re
import sys
from typing import List, Any

class gerrit():
    def __init__(self, hostname: str = None, port: int = 0, username: str = None) -> None:
        """
        Initiate a gerrit handler instance.
        If no hostname or port is given, we will see if we're in a git repo that lets us deduce the host, port
        (and possibly username).
        If no username is given, we will try to use the user variable in the environment.
        """
        if not hostname or port == 0:
            try:
                remote_str = self._run_local_git(["remote", "get-url", "origin"])
            except Exception as e:
                if not os.path.exists(".git"):
                    raise RuntimeError("I can't see any git repository here")
                raise e
            if "ssh://" in remote_str:
                match = re.search(r"ssh://(.*)@(.*):(\d+).*", remote_str)
                if match:
                    self.username, self.hostname, lport = match.group(1, 2, 3)
                    self.port = int(lport)
            else:
                raise RuntimeError("Unrecognized git remote url: {}".format(remote_str))
        else:
            self.hostname = hostname
            self.port = port
            self.username = username
            if not username:
                self.username = os.environ['USER']

    def _run_ssh(self, command: List[str]) -> str:
        #print("ssh -p{} {}@{} {}".format(self.port, self.username, self.hostname, " ".join(command)))
        output = subprocess.check_output(["ssh",
                                          "-p{}".format(self.port),
                                          "{}@{}".format(self.username, self.hostname)] + command)
        return output.decode()

    def _run_local_git(self, command: List[str]) -> str:
        command.insert(0, "git")
        return subprocess.check_output(command).decode()

    def _run_remote_gerrit(self, command: List[str]) -> str:
        return self._run_ssh(["gerrit"] + command)

    def _run_remote_gerrit_json_response(self, command: List[str]) -> Any:
        response = self._run_ssh(["gerrit"] + command)
        return json.loads("[" + ",".join(response.splitlines()) + "]")

    def ls_projects(self) -> List[str]:
        """
        Returns a list projects on gerrit server
        """
        return self._run_remote_gerrit(["ls-projects"]).splitlines()

    def query(self, query_str: List[str]) -> Any:
        """
        Returns a json object with the response to the query.
        See https://review.openstack.org/Documentation/cmd-query.html for query format
        """
        return self._run_remote_gerrit_json_response(["query", "--format", "JSON"] + query_str)

    def review(self, args: List[str]):
        """
        Set review/label status. See https://review.openstack.org/Documentation/cmd-review.html
        for parameter format.
        """
        self._run_remote_gerrit(["review"] + args)

