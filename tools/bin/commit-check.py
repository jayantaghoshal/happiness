#!/usr/bin/env python3

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
commit-check

Usage:
  commit-check verify <repo-root>
  commit-check fix <repo-root>
  commit-check pre-commit <repo-root>


Examples:
  commit-check fix /directory
  commit-check verify .

Help:
  This tool support commit check implementation and fixing fixable verifications

"""

import commitcheck
from ihuutils.docopt_dispatch import dispatch


@dispatch.on('verify')
def verify(repo_root, **kwargs):
    commitcheck.verify_repo(repo_root)


@dispatch.on('fix')
def fix(repo_root, **kwargs):
    commitcheck.fix_repo(repo_root)


@dispatch.on('pre-commit')
def precommit(repo_root, **kwargs):
    commitcheck.pre_commit_hook(repo_root)


if __name__ == "__main__" and __package__ is None:
    dispatch(__doc__)
