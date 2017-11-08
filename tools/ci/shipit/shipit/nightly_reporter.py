import html
from typing import Iterable, Set
from xml.etree import cElementTree as ET

import subprocess

from shipit import process_tools, git, smtp
from collections import namedtuple
import shlex

_Config = namedtuple("Config", "vote branch")

BUILD_CONFIG = {
    "ihu_image_build": _Config(1, "ci/last_successful_hourly_build"),
    "ihu_hourly_test": _Config(2, "ci/last_successful_hourly_test"),
    "ihu_nightly_build": _Config(3, "ci/last_successful_nightly_build"),
    "ihu_nightly_test": _Config(4, "ci/last_successful_nightly_test")
}

DISALLOWED_EMAILS = {
    "zuul@icup_android.gerrit.cm.volvocars.biz",
    "no-reply@volvocars.com"
}


def _gerrit_vote_confidence_level(commit_hash: str, message: str, cl_vote: int):
    # https://gerrit-documentation.storage.googleapis.com/Documentation/2.14.5.1/cmd-review.html

    try:
        # NOTE: Need double escaping because we run a command inside ssh
        process_tools.check_output_logged(
            ["ssh",
             "-p", "29421",
             "gotsvl1415.got.volvocars.net",
             "gerrit", "review",
             "--project", "vendor/volvocars",
             "--message", shlex.quote(message),
             "--label", shlex.quote("Confidence-Level=%d" % cl_vote),
             shlex.quote(commit_hash)]
        )
    except subprocess.CalledProcessError as cp:
        # Silently ignore these as zuul can create commits without corresponding gerrit change
        if "no such patch set" not in cp.stderr.decode():
            raise


def _collect_commit_hashes(repo: git.Repo, commit_range: str) -> Iterable[str]:
    lines = repo.run_git(["log", commit_range, "--format=%H"])
    for l in lines.splitlines():
        yield l.strip()


def _get_volvocars_diff_in_manifest_since(branch: str):
    manifest_repo = git.Repo("../../../../../.repo/manifests")
    manifest_repo.run_git(['fetch', '--all'])
    A = manifest_repo.show("origin/%s:manifest-volvocars.xml" % branch)
    B = manifest_repo.show('HEAD:manifest-volvocars.xml')

    et = ET.fromstring(A)
    project = et.find("./project[@path='vendor/volvocars']")
    revisionA = project.attrib['revision']

    et = ET.fromstring(B)
    project = et.find("./project[@path='vendor/volvocars']")
    revisionB = project.attrib['revision']

    return revisionA + ".." + revisionB


def report_build_success(jenkins_job_name, jenkins_build_nr):
    log_link = "https://icup_android.jenkins.cm.volvocars.biz/job/%s/%s/console" % (jenkins_job_name, jenkins_build_nr)
    config = BUILD_CONFIG[jenkins_job_name]

    # Get revision_range must be done before pushing branch
    revision_range = _get_volvocars_diff_in_manifest_since(config.branch)

    # Send status-vote in gerrit
    volvocars_repo = git.Repo("../../../")
    commits = _collect_commit_hashes(volvocars_repo, revision_range)
    for c in commits:
        _gerrit_vote_confidence_level(c, jenkins_job_name + " " + log_link, config.vote)

    # Push the ci/xxx branch
    manifest_repo = git.Repo("../../../../../.repo/manifests")
    manifest_repo.run_git(['fetch', '--all'])
    revMaster = manifest_repo.run_git(['rev-parse', 'HEAD']).strip()
    manifest_repo.run_git(['branch', '-f', config.branch, revMaster])
    manifest_repo.run_git(['push', '-f', 'origin', config.branch])


def report_build_failed(jenkins_job_name, jenkins_build_nr):
    config = BUILD_CONFIG[jenkins_job_name]
    log_link = "https://icup_android.jenkins.cm.volvocars.biz/job/%s/%s/console" % (jenkins_job_name, jenkins_build_nr)

    revision_range = _get_volvocars_diff_in_manifest_since(config.branch)

    volvocars_repo = git.Repo("../../../")

    # Send status-vote in gerrit
    commits = _collect_commit_hashes(volvocars_repo, revision_range)
    for c in commits:
        _gerrit_vote_confidence_level(c, jenkins_job_name + " " + log_link, config.vote * -1)

    # Send Email
    volvocars_repo.run_git(['fetch', '--all'])
    email_log = volvocars_repo.run_git(['log', revision_range, '--format=%ae']).strip()
    gitlog = volvocars_repo.run_git(['log', revision_range, '--format=email'])
    gitlog = gitlog.replace('From', '- From')  # outlook365 hack
    files_changed = volvocars_repo.run_git(['diff', revision_range, '--name-only'])
    email_addreses = set(email_log.splitlines()) - DISALLOWED_EMAILS
    _send_build_failed_email(
        gitlog,
        files_changed,
        email_addreses,
        jenkins_job_name,
        jenkins_build_nr)


def _send_build_failed_email(log: str, files: str, to_list: Set[str], job_name: str, log_link: str):
    # TODO: Enable email sending once master has been stabilized, right now too many test cases failing
    if True:
        return

    template = """
        <h1>Build result</h1>
        %(type_of_test)s has failed since you commited changes to master.
        <br />
        Log: <a href=%(log_link)s>%(log_link)s</a>
        <h2>Files changed since last working %(type_of_test)s test: </h2>
        <pre>%(files_last_working)s </pre>
        <h2>List of commits since last successful</h2>
        <pre>%(commit_list)s </pre>
        """

    html_contents = template % {
        "log_link": html.escape(log_link),
        "files_last_working": html.escape(files),
        "commit_list": html.escape(log),
        "type_of_test": html.escape(job_name)
    }

    subject = job_name + " failing"

    smtp.send_email(to_list, subject, html_contents)
