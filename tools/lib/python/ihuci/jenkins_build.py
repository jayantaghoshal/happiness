from datetime import datetime, timedelta
import time

class JenkinsBuild(object):
    
    @classmethod
    def create(cls, build):
        jenkins_build = cls()
        jenkins_build.parse(build)
        return jenkins_build

    def __init__(self):
        self.number = None
        self.result = None
        self.time = None
        self.repo = None
        self.branch = None
        self.author = None
        self.email = None
        self.commit_id = None
        self.zuul_change_ids = None
        self.message = None
        self.url = None
        self.host = None
        self.duration = None

    def parse(self, build):
        self.number = build["id"]
        self.result = "PENDING" if build["result"] is None else build["result"]
        self.time = datetime.fromtimestamp(build["timestamp"]/1000.0).strftime("%Y-%m-%dT%H:%M:%SZ")
        self.host = build["builtOn"]
        self.duration = self._duration(build)
        for change_set in build["changeSet"]["items"]:
            self.author = change_set["authorEmail"]
            self.message = change_set["msg"][0:49]
            self.commit_id = change_set["commitId"][0:7]
        for action in build["actions"]:
            if "parameters" in action:
                for parameter in action["parameters"]:
                    if (parameter["name"] == "GERRIT_BRANCH") or (parameter["name"] == "ZUUL_BRANCH"):
                        self.branch = parameter["value"]
                    if (parameter["name"] == "GERRIT_PROJECT") or (parameter["name"] == "ZUUL_PROJECT"):
                        self.repo = parameter["value"]
                    if parameter["name"] == "GERRIT_CHANGE_URL":
                        self.url = parameter["value"]
                    if (parameter["name"] == "GERRIT_NEWREV") or (parameter["name"] == "ZUUL_COMMIT"):
                        self.commit_id = parameter["value"][0:7]
                    if parameter["name"] == "ZUUL_PROJECT":
                        self.repo = parameter["value"]
                    if parameter["name"] == "ZUUL_CHANGE_IDS":
                        self.zuul_change_ids = parameter["value"]
                    if parameter["name"] == "GERRIT_CHANGE_SUBJECT":
                        self.message = parameter["value"][0:49]
                    if parameter["name"] == "GERRIT_CHANGE_OWNER_EMAIL":
                        self.email = parameter["value"]
                    if parameter["name"] == "GERRIT_CHANGE_OWNER_NAME":
                        self.author = parameter["value"]
                        
    def _duration(self, build):
        duration = None
        if (build["duration"] is None) or (build["duration"] == 0):
            tdelta = datetime.now() - datetime.fromtimestamp(build["timestamp"]/1000.0)
            duration = time.strftime("%H:%M:%S", time.gmtime(tdelta.seconds))
        else:
            duration = time.strftime("%H:%M:%S", time.gmtime(build["duration"]/1000.0))
        return duration
