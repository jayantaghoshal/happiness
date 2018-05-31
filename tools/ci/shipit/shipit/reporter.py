# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import html
from typing import Iterable, Set
from xml.etree import cElementTree as ET
import subprocess
from shipit import process_tools, git, smtp, paths
from collections import namedtuple
import shlex
import logging

logger = logging.getLogger(__name__)

SUCCESS_BRANCHES = {
    "ihu_hourly_build-eng": "ci/last_successful_hourly_build",
    #"ihu_hourly_test": "ci/last_successful_hourly_test",     #Not used
    "ihu_daily_build_vcc_eng": "ci/last_successful_nightly_build",
    #"ihu_nightly_test": "ci/last_successful_nightly_test",   #Not used
    "ihu_hourly": "ci/last_successful_hourly",
    "ihu_daily": "ci/last_successful_daily",
    "ihu_weekly": "ci/last_successful_weekly", #Not used
}

LAST_BRANCHES = {
    #"ihu_hourly_build-eng": "ci/last_run_hourly_build",
    #"ihu_daily_build_vcc_eng": "ci/last_run_nightly_build",
    "ihu_hourly": "ci/last_run_hourly",
    "ihu_daily": "ci/last_run_daily",
    "ihu_weekly": "ci/last_run_weekly", #Not used
}

DISALLOWED_EMAILS = {
    "zuul@icup_android.gerrit.cm.volvocars.biz",
    "no-reply@volvocars.com"
}


def _make_filtered_email_list(email_list: Set[str]):
    list_out = {x for x in email_list if x not in DISALLOWED_EMAILS and x.endswith('@volvocars.com')}
    return list_out


def _send_build_failed_email(email_addresses: Set[str], job_name: str, build_number: str, committer_list: Iterable[str]=set()):
    result_link = "http://gotsvl1416.got.volvocars.net:3001/tests/?top_job_name=" + job_name + "&build_number=" + build_number
    jenkins_link = "https://icup_android.jenkins.cm.volvocars.biz/job/" + job_name + "/" + build_number + "/"

    template = '''
        <h1>Build result</h1>
        {job_name} has failed. Possibly because of your commit.<br>
        <h2>More information</h2>
        Results: <a href="{result_link}">{result_link}</a><br>
        Jenkins: <a href="{jenkins_link}">{jenkins_link}</a>
        '''

    substitutes = {
        "result_link": html.escape(result_link),
        "jenkins_link": html.escape(jenkins_link),
        "job_name": html.escape(job_name)
    }
    html_contents = template.format(**substitutes)
    if committer_list:
        html_contents += '''
        <h2>Committers</h2>\n'''
        for committer in committer_list:
            html_contents += html.escape(committer) + "<br>\n"

    subject = job_name + " failing"

    smtp.send_email(email_addresses, subject, html_contents)


def _gerrit_vote_confidence_level(commit_hash: str, message: str, cl_vote: int):
    # TODO: Requires Gerrit version 2.14, we only have 2.13, for https://gerrit-review.googlesource.com/Documentation/config-labels.html#label_allowPostSubmit
    return
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


def _get_volvocars_diff_in_manifest_since(branch: str, current: str="HEAD"):
    manifest_repo = git.Repo(paths.manifest())
    manifest_repo.run_git(['fetch', '--all'])
    A = manifest_repo.show("origin/%s:manifest-volvocars.xml" % branch)
    B = manifest_repo.show(current + ':manifest-volvocars.xml')

    et = ET.fromstring(A)
    project = et.find("./project[@path='vendor/volvocars']")
    revisionA = project.attrib['revision']

    et = ET.fromstring(B)
    project = et.find("./project[@path='vendor/volvocars']")
    revisionB = project.attrib['revision']

    return revisionA + ".." + revisionB


def _make_committer_list(from_branch: str, to_git_hash: str):
    revision_range = _get_volvocars_diff_in_manifest_since(from_branch, to_git_hash)
    volvocars_repo = git.Repo(paths.volvocars())
    volvocars_repo.run_git(['fetch', '--all'])
    commit_log = volvocars_repo.run_git(['log', revision_range, '--format=%an, %ae']).strip()
    committer_list = set(commit_log.splitlines())
    return committer_list


def update_manifest_branches(jenkins_job_name, status, git_hash: str="HEAD"):
    if status == 'pass' and jenkins_job_name in SUCCESS_BRANCHES:
        branch = SUCCESS_BRANCHES[jenkins_job_name]
        logger.info("Updating branch: {}".format(branch))
        update_manifest_branch(branch, git_hash)
    if jenkins_job_name in LAST_BRANCHES:
        branch = LAST_BRANCHES[jenkins_job_name]
        logger.info("Updating branch: {}".format(branch))
        update_manifest_branch(branch, git_hash)


def update_manifest_branch(branch: str, git_hash: str='HEAD'):
    manifest_repo = git.Repo(paths.manifest())
    manifest_repo.run_git(['fetch', '--all'])
    revMaster = manifest_repo.run_git(['rev-parse', git_hash]).strip()
    manifest_repo.run_git(['branch', '-f', branch, revMaster])
    manifest_repo.run_git(['push', '-f', 'origin', branch])


def email_committers_since_last(jenkins_job_name: str, jenkins_job_number: str, git_hash: str="HEAD"):
    email_committers_since_branch(LAST_BRANCHES[jenkins_job_name], jenkins_job_name, jenkins_job_number, git_hash)


def email_committers_since_success(jenkins_job_name: str, jenkins_job_number: str, git_hash: str="HEAD"):
    email_committers_since_branch(SUCCESS_BRANCHES[jenkins_job_name], jenkins_job_name, jenkins_job_number, git_hash)


def email_committers_since_branch(branch: str, jenkins_job_name: str, jenkins_job_number: str, git_hash: str="HEAD"):
    try:
        revision_range = _get_volvocars_diff_in_manifest_since(branch, git_hash)
    except git.Error as e:
        if 'fatal: Invalid object name' in str(e): #Branch does not exist
            print(e)
            return
        else:
            raise
    volvocars_repo = git.Repo(paths.volvocars())
    volvocars_repo.run_git(['fetch', '--all'])
    email_log = volvocars_repo.run_git(['log', revision_range, '--format=%ae']).strip()
    email_addresses = _make_filtered_email_list(set(email_log.splitlines()))

    if not email_addresses: #Possible that there was no one to email
        return

    committer_list = _make_committer_list(branch, git_hash)
    _send_build_failed_email(
        email_addresses,
        jenkins_job_name,
        jenkins_job_number,
        committer_list)


def email_list_of_users(email_list: Set[str], jenkins_job_name: str, jenkins_job_number: str, git_hash: str="HEAD"):
    email_addresses = _make_filtered_email_list(email_list)
    committer_list = _make_committer_list(LAST_BRANCHES[jenkins_job_name], git_hash)
    _send_build_failed_email(
        email_addresses,
        jenkins_job_name,
        jenkins_job_number,
        committer_list)


def set_gerrit_confidence_level(jenkins_job_name: str, jenkins_build_nr: str, status: str, git_hash: str="HEAD"):
    #TODO: fix votes to "real" values
    if status == 'pass':
        vote = 1
    else:
        vote = 0

    branch = SUCCESS_BRANCHES[jenkins_job_name]
    log_link = "https://icup_android.jenkins.cm.volvocars.biz/job/%s/%s/console" % (jenkins_job_name, jenkins_build_nr)

    revision_range = _get_volvocars_diff_in_manifest_since(branch, git_hash)
    volvocars_repo = git.Repo(paths.volvocars())

    # Send status-vote in gerrit
    commits = _collect_commit_hashes(volvocars_repo, revision_range)
    for c in commits:
        _gerrit_vote_confidence_level(c, jenkins_job_name + " " + log_link, vote)


