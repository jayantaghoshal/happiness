#!/usr/bin/env python3

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import collections
import subprocess
import os
import typing
import codeformat
import licensingcomponents
import linters
import sys
from licensingcomponents import copyrightheader

ErrorClassBanners = {
    linters.ShellcheckLinterError:
        """
-------------------------------------------------------------------"
SHELLCHECK Analysis
-------------------------------------------------------------------

This will analyze all shell-scripts in volvocars repository

  * Please take warnings seriously
  * If you don't understand the warning:
      1. Copy the warning code, eg SC1001
      2. Go to https://github.com/koalaman/shellcheck/wiki/###### where ##### is to be replaced with the warning code
      3. Google the warning code
  * If you understand the warning and think it can be ignored, read https://github.com/koalaman/shellcheck/wiki/Ignore
-------------------------------------------------------------------""",

    linters.AndroidLoggingLinterError:
        """
-------------------------------------------------------------------"
Android Native Logging Analysis
-------------------------------------------------------------------

This checks/enforces guidelines for using Android Logging in
VCC native components. For details see:

https://c1.confluence.cm.volvocars.biz/display/IHUA/Logging
-------------------------------------------------------------------""",

    codeformat.CodeFormatInvalidError:
        """
-------------------------------------------------------------------
Automatic code formatting
-------------------------------------------------------------------

This will ensure correct code style for all supported files in repository
  * Adherence to decided style is mandatory
  * If no good workaround exists you can add //clang-format on/off guards
  * This should not happen if commit hook was properly installed
-------------------------------------------------------------------""",

    licensingcomponents.copyrightheader.LicenseHeaderInvalidError:
        """
-------------------------------------------------------------------
Automatic LICENSE file headers
-------------------------------------------------------------------

This will ensure correct license header in all files where it is
applicable to mention the relevant LICENSE file in repository.

This should be handled autonomously during commit phase
if commit hook was properly installed
-------------------------------------------------------------------""",

}


def collect_loop_exceptions(item_list, action_on_item, accepted_types=(Exception,)):
    error_list = []

    for item in item_list:
        try:
            action_on_item(item)
        except accepted_types as ex:
            error_list.append(ex)

    return error_list


def print_error_explanations(error_list: typing.Iterable[Exception]):
    sorted_error_list = sorted(error_list, key=lambda er: type(er).__name__)

    grouped_errors = collections.defaultdict(list)  # type: typing.DefaultDict[type, typing.List[Exception]]
    for e in sorted_error_list:
        grouped_errors[type(e)].append(e)

    for group, errors_in_group in grouped_errors.items():
        if group in ErrorClassBanners:
            print(ErrorClassBanners[group])
            print("Found {} violations:".format(len(errors_in_group)))
        else:
            raise Exception("Unknown type of error collected - {}".format(group))
        for eig in errors_in_group:
            print(eig)


def aggregate_and_exit_with_report_on_failure(item_list, action_on_item,
                                              error_processor=print_error_explanations,
                                              error_types_tuple=(Exception,)):
    error_list = collect_loop_exceptions(item_list, action_on_item, accepted_types=error_types_tuple)

    print("Processing completed, found {} issues".format(len(error_list)))
    if len(error_list) > 0:
        error_processor(error_list)
        sys.exit(1)


def build_git_file_list(repo_root: str,
                        file_class_description: str,
                        git_command_with_args_for_file_class: typing.List[str]):
    repo_abs_dir = os.path.abspath(repo_root)
    print("Finding {} files in {}...exit".format(file_class_description, repo_abs_dir))

    git_output = subprocess.check_output(git_command_with_args_for_file_class, cwd=repo_abs_dir).decode()
    git_files = git_output.splitlines(keepends=False)

    count = 0
    for file in git_files:
        abs_file_path = os.path.abspath(os.path.join(repo_abs_dir, file))
        count = count + 1
        yield abs_file_path

    print("Found {} of {} files in {}".format(count, file_class_description, repo_abs_dir))


def build_git_controlled_file_list(repo_root):
    return build_git_file_list(repo_root,
                               "Git controlled",
                               ['git', 'ls-tree', '-r', 'HEAD', '--name-only'])


def build_git_controlled_changed_file_list(repo_root):
    return build_git_file_list(repo_root,
                               "Git controlled and changed",
                               ['git', 'diff', 'HEAD', '--name-only', '--diff-filter=ACMRT'])


ExpectedExceptions = (linters.LinterError,
                      codeformat.CodeFormatInvalidError,
                      copyrightheader.LicenseHeaderInvalidError)


def is_external_thirdparty(file_path: str) -> bool:
    if file_path.find("/external/") > 0:
        return True

    if file_path.find("/thirdparty/") > 0:
        return True

    return False


def verify_file_path(file_path):
    if not is_external_thirdparty(file_path):
        codeformat.verify_source_file_format(file_path)
        copyrightheader.verify_copyright_headers(file_path)
        linters.run_for_file(file_path)


def fix_file_path(file_path):
    if not is_external_thirdparty(file_path):
        codeformat.apply_source_file_format(file_path)
        copyrightheader.fix_copyright_headers(file_path)
        linters.run_for_file(file_path)


def verify_repo(repo_root, **kwargs):
    paths = build_git_controlled_file_list(repo_root)
    aggregate_and_exit_with_report_on_failure(paths, verify_file_path, error_types_tuple=ExpectedExceptions)


def fix_repo(repo_root, **kwargs):
    paths = build_git_controlled_file_list(repo_root)
    aggregate_and_exit_with_report_on_failure(paths, fix_file_path, error_types_tuple=ExpectedExceptions)


def precommit_hook(repo_root, **kwargs):
    paths = build_git_controlled_changed_file_list(repo_root)
    aggregate_and_exit_with_report_on_failure(paths, fix_file_path, error_types_tuple=ExpectedExceptions)
