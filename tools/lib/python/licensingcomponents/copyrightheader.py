# coding=utf-8

# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import typing
import collections
import os
import re
from datetime import datetime
from ihuutils.sourcecode import sanitize_whitespace


class LicenseHeaderNeededUpdateError(Exception):
    def __init__(self, source_file):
        # Call the base class constructor with the parameters it needs
        super(LicenseHeaderNeededUpdateError, self) \
            .__init__("Copyright header needed to be fixed in file {}".format(source_file))


class LicenseHeaderInvalidError(Exception):
    def __init__(self, violation: str, source_file: str):
        # Call the base class constructor with the parameters it needs
        super(LicenseHeaderInvalidError, self).__init__("{} in file {}".format(violation, source_file))


class LicenseHeaderMultipleCopyrightHoldersError(Exception):
    def __init__(self, copyright_headers: typing.Iterable[str], source_file: str):
        # Call the base class constructor with the parameters it needs
        super(LicenseHeaderMultipleCopyrightHoldersError, self) \
            .__init__("Multiple Copyright headers found {} in file {}".format(copyright_headers, source_file))


LanguageCommentFeatures = collections.namedtuple('LanguageCommentFeatures', ['firstline',
                                                                             'endline',
                                                                             'bodyline_prefix',
                                                                             'bodyline_suffix',
                                                                             'headers_info',
                                                                             'headers_matcher',
                                                                             'headers_to_replace',
                                                                             'line_patterns_allowed_before_copyright'])

CopyrightInfo = collections.namedtuple("CopyrightInfo", ['start_year',
                                                         'end_year',
                                                         'company'])

CStyleInfoRegex = re.compile(r'^/\*\n\s\*\sCopyright\s(?P<start_year>20\d{2})'
                             r'(?:-(?P<end_year>20\d{2}))?\s(?P<company>Delphi|Volvo)\s.*?(?:\n\s\*.*?){2}/',
                             re.IGNORECASE)
CStyleGenericRegex = re.compile(r'^\s\*\s(?:Copyright.*)',
                                re.IGNORECASE | re.MULTILINE)
CStyleReplaceableRegex = re.compile(r'(?:^/\*\n\s\*\sCopyright.*(?:Delphi|Volvo)(?:.*\n)*?\s\*/)',
                                    re.IGNORECASE)

CLangFeatures = LanguageCommentFeatures(firstline='/*\n',
                                        endline=' */\n\n',
                                        bodyline_prefix=' * ',
                                        bodyline_suffix='\n',
                                        headers_info=[CStyleInfoRegex],
                                        headers_matcher=[CStyleGenericRegex],
                                        headers_to_replace=[CStyleReplaceableRegex],
                                        line_patterns_allowed_before_copyright=[]
                                        )

HashStartInfoRegex = re.compile(r'^#\sCopyright\s(?P<start_year>20\d{2})'
                                r'(?:-(?P<end_year>20\d{2}))?\s(?P<company>Delphi|Volvo)\s.*',
                                re.IGNORECASE)
HashStartGenericRegex = re.compile(r'^#\s(?:Copyright.*)',
                                   re.IGNORECASE | re.MULTILINE)
HashStartReplaceableRegex = re.compile(r'(?:^#\s(?:Copyright.*?(?:Delphi|Volvo).*?\n)(?:#.*?\n)+)',
                                       re.IGNORECASE)
HashStartLanguageFeatures = LanguageCommentFeatures(firstline='',
                                                    endline='\n',
                                                    bodyline_prefix='# ',
                                                    bodyline_suffix='\n',
                                                    headers_info=[HashStartInfoRegex],
                                                    headers_matcher=[HashStartGenericRegex],
                                                    headers_to_replace=[HashStartReplaceableRegex],
                                                    line_patterns_allowed_before_copyright=[]
                                                    )

HashStartLanguageWithShebangFeatures = HashStartLanguageFeatures._replace(
    line_patterns_allowed_before_copyright=["^#!.*", "^[ \t\v]*#.*?coding[:=][ \t]*([-_.a-zA-Z0-9]+)"])

CommentPattern = "Copyright {year} Volvo Car Corporation\n" \
                 "This file is covered by LICENSE file in the root of this project"

KnownLanguages = {
    '.c': CLangFeatures,
    '.h': CLangFeatures,
    '.cpp': CLangFeatures,
    '.java': CLangFeatures,
    '.bp': CLangFeatures,
    '.mk': HashStartLanguageFeatures,
    '.py': HashStartLanguageWithShebangFeatures,
    '.sh': HashStartLanguageWithShebangFeatures,
}

PathsIgnoreRegex = re.compile(r'.*(?:'
                              r'(?:__init__.py)|'
                              r'(?:vendor/volvocars/interfaces/.*\.(?:bp|mk))'
                              r')')


def should_filepath_be_ignored(path: str):
    abspath = os.path.abspath(path)

    filename, extension = os.path.splitext(path)
    if extension not in get_known_languages_extensions():
        return True

    return PathsIgnoreRegex.match(abspath) is not None


def filter_ignored(files):
    for f in files:
        if not should_filepath_be_ignored(f):
            yield f


def get_known_languages_extensions():
    return KnownLanguages.keys()


def __get_copyright_headers_from_body(file_body: str, lang: LanguageCommentFeatures) -> typing.List[str]:
    headers = []
    for regex in lang.headers_matcher:
        for m in regex.findall(file_body):
            headers.append(m)
    return headers


def list_headers_and_filenames(file_path: str):
    lang = get_lang(file_path)

    with open(file_path, encoding="utf-8") as file:
        file_body = file.read()

    headers = __get_copyright_headers_from_body(file_body, lang)
    if headers:
        print(file_path)
        for h in headers:
            print(h)


def get_lang(file_path: str) -> LanguageCommentFeatures:
    ext = os.path.splitext(file_path)[1]
    if ext not in KnownLanguages:
        raise Exception("File {0} is of not supported language {1}".format(file_path, ext))

    lang = KnownLanguages.get(ext)
    return lang


def __get_copyright_info_from_body(file_body: str, lang: LanguageCommentFeatures) -> typing.Optional[CopyrightInfo]:
    """ Get Copyright information from the file_body content

    Args:
        file_body: File content to get copyright information from
        lang: Properties for the language used in the file_body's source file

    Returns:
        A CopyrightInfo tuple with the year and company name from the file_body

        None if no matching Copyright header was found in the file_body
    """
    assert isinstance(lang, LanguageCommentFeatures)

    match = None
    for regex in lang.headers_info:
        match = regex.search(file_body)
        if match is not None:
            break

    if match is None:
        return None

    return CopyrightInfo(start_year=match.group('start_year'),
                         end_year=match.group('end_year'),
                         company=match.group('company'))


def __remove_header_from_body(file_body: str, lang: LanguageCommentFeatures) -> str:
    assert isinstance(lang, LanguageCommentFeatures)

    file_body_without_headers = file_body
    for regex in lang.headers_to_replace:
        file_body_without_headers = regex.sub('', file_body_without_headers)

    file_body_without_headers = sanitize_whitespace(file_body_without_headers)

    return file_body_without_headers


def __add_header_to_body(file_body: str, copyright_year: str, lang: LanguageCommentFeatures) -> str:
    copyright_header = lang.firstline

    for line in CommentPattern.splitlines(keepends=False):
        copyright_header += lang.bodyline_prefix
        copyright_header += line.replace('{year}', copyright_year)
        copyright_header += lang.bodyline_suffix

    copyright_header += lang.endline

    header_after_removal_processing = __remove_header_from_body(copyright_header, lang)
    if header_after_removal_processing != "":
        raise Exception("Internal error, generated header would not be removable")

    return copyright_header + file_body


def __filter_lines_before_copyright(file_body: str, lang: LanguageCommentFeatures):
    """ Filter the lines in the file body before the copyright as specified by lang
        to avoid moving them below copy header which would break their purpose.

        Args:
            file_body: File content to filter
            lang: Properties for the language used in the file_body's source file

        Returns:
            Two values, the first one is a list of all the lines that has been filtered from the file body and the
            second value is a string with the filtered file body.
    """
    saved_lines_before_copyright = []
    file_lines = file_body.splitlines(keepends=True)  # keepends to preserve potential trailing newline end of file
    filtered_file_body = ""
    scanning_allowed_lines = True
    for line in file_lines:
        if scanning_allowed_lines:
            line_ok = False
            for allowed_line_pattern in lang.line_patterns_allowed_before_copyright:
                if re.match(allowed_line_pattern, line):
                    saved_lines_before_copyright.append(line)
                    line_ok = True
                    break
            if not line_ok:
                scanning_allowed_lines = False

        if not scanning_allowed_lines:
            filtered_file_body += line

    return saved_lines_before_copyright, filtered_file_body


def __replace_headers_in_body(file_body: str, file_path: str, lang: LanguageCommentFeatures):
    removed_headers, filtered_file_body = __filter_lines_before_copyright(file_body, lang)

    # Remove trailing whitespace after the removed headers, needed for reproducibility
    filtered_file_body = sanitize_whitespace(filtered_file_body)

    current_year = datetime.now().year
    copyright_year = None
    copyright_headers = __get_copyright_headers_from_body(filtered_file_body, lang)

    if len(copyright_headers) > 1:
        raise LicenseHeaderMultipleCopyrightHoldersError(copyright_headers, file_path)
    elif copyright_headers:
        info = __get_copyright_info_from_body(filtered_file_body, lang)
        if info is None:
            raise LicenseHeaderInvalidError("Unrecognized copyright found {}".format(copyright_headers), file_path)
        else:
            if info.company == "Delphi":
                # If the company is Delphi a new copyright header shall be written
                copyright_year = str(current_year)
            elif current_year > int(info.start_year):
                # If the Copyright header contains an older year update the header with a range from then to now
                copyright_year = "{0}-{1}".format(info.start_year, str(current_year))

    # If none of the above clauses apply write the current year in the Copyright header
    if copyright_year is None:
        copyright_year = str(current_year)

    decopyrighted = __remove_header_from_body(filtered_file_body, lang)
    # Remove trailing whitespace after the removed headers, needed for reproducibility
    decopyrighted = sanitize_whitespace(decopyrighted)

    with_copyright_applied = __add_header_to_body(decopyrighted, copyright_year, lang)
    if len(removed_headers) > 0:
        removed_headers.append("\n")
    with_copyright_and_saved_lines_applied = "".join(removed_headers + [with_copyright_applied])
    return with_copyright_and_saved_lines_applied


def get_contents_with_header_applied(file_path: str):
    assert isinstance(file_path, str)
    file_path = os.path.abspath(file_path)

    lang = get_lang(file_path)

    with open(file_path, encoding="utf-8") as file:
        file_body = file.read()

    with_copyright_applied = __replace_headers_in_body(file_body, file_path, lang)
    with_copyright_applied_twice = __replace_headers_in_body(with_copyright_applied, file_path, lang)

    if with_copyright_applied_twice != with_copyright_applied:
        with open(file_path + ".iter_orig", "w") as copy_not_applied:
            copy_not_applied.write(file_body)

        with open(file_path + ".iter1", "w") as copy_applied_once:
            copy_applied_once.write(with_copyright_applied)

        with open(file_path + ".iter2", "w") as copy_applied_twice:
            copy_applied_twice.write(with_copyright_applied_twice)

        raise Exception("Internal error, non reproducible application of header to file {},"
                        "Please commit the change with git commit --no-verify and reference it "
                        "in JIRA issue assigned to krzysztof.wesolowski@volvocars.com or CI team".format(file_path))

    return with_copyright_applied, file_body


def verify_copyright_headers(file_path: str) -> None:
    """ Verify the Copyright header in the specified file to be at least:
        a) present,
        b) pointing VCC
        c) not pointing other company.

    :param file_path: The file to verify
    :raises LicenseHeaderInvalidError: If the Copyright header is invalid or missing
    :raises LicenseHeaderMultipleCopyrightHoldersError if there is more than one copyright header
    """
    assert isinstance(file_path, str)
    file_path = os.path.abspath(file_path)
    if should_filepath_be_ignored(file_path):
        return;

    with open(file_path, encoding="utf-8") as file:
        file_body = file.read()

    lang = get_lang(file_path)

    _, filtered_file_body = __filter_lines_before_copyright(file_body, lang)
    filtered_file_body = sanitize_whitespace(filtered_file_body)
    copyright_headers = __get_copyright_headers_from_body(filtered_file_body, lang)

    if not copyright_headers:
        raise LicenseHeaderInvalidError("Missing Copyright header", file_path)
    elif len(copyright_headers) > 1:
        raise LicenseHeaderMultipleCopyrightHoldersError(copyright_headers, file_path)

    info = __get_copyright_info_from_body(filtered_file_body, lang)
    current_year = datetime.now().year
    if info is None:
        # Copyright head has unknown format
        raise LicenseHeaderInvalidError("Unrecognized copyright found {}".format(copyright_headers), file_path)
    elif int(info.start_year) < 2017:
        # The old script was hard coded to only accept 2017, so copyright before that is not allowed
        raise LicenseHeaderInvalidError("Copyright header invalid year", file_path)
    elif info.end_year is not None:
        # If the Copyright header contains a year range verify that the range is valid
        if int(info.end_year) <= int(info.start_year) or current_year < int(info.end_year):
            raise LicenseHeaderInvalidError("Copyright header invalid year range", file_path)


def update_copyright_headers(file_path: str) -> None:
    """
    Update copyright header to  contain proper company and date range

    :param file_path: One files to be updated
    :raises LicenseHeaderInvalidError if file needed to be updated
    """
    assert os.path.isfile(file_path)
    fixed, original = get_contents_with_header_applied(file_path)
    if fixed != original:
        with(open(file_path, 'w', encoding="utf-8")) as target:
            target.write(fixed)
        raise LicenseHeaderNeededUpdateError(file_path)


def fix_copyright_headers(file_path: str) -> None:
    """
    Ensures all files would pass subsequent verify call

    :param file_path: File to be fixed fixed (updated if fails verification)
    :raises LicenseHeaderInvalidError if any file needed fixing
    :raises LicenseHeaderDuplicateError if file contains duplicate copyright and cannot be fixed
    """
    try:
        verify_copyright_headers(file_path)
    except LicenseHeaderInvalidError:
        update_copyright_headers(file_path)


def collect_supported_files(directory: str) -> typing.Iterable[str]:
    for entry in os.scandir(directory):
        if entry.is_dir():
            yield from collect_supported_files(entry.path)

        if entry.is_file():
            filename, ext = os.path.splitext(entry.path)
            if ext in get_known_languages_extensions():
                yield entry.path
    pass


def collect_copyrightable_files(directory: str) -> typing.Iterable[str]:
    yield from filter_ignored(collect_supported_files(directory))


def list_copyright_headers(file_or_files_list: typing.Union[str, typing.Iterable[str]]):
    files = __to_file_list(file_or_files_list)

    for f in files:
        if should_filepath_be_ignored(f):
            print("Ignoring file {}".format(f))
            continue

        list_headers_and_filenames(f)


def fix_all_copyright_headers(directory):
    for path in collect_copyrightable_files(directory):
        update_copyright_headers(path)


def __to_file_list(file_or_files_list: typing.Union[str, typing.Iterable[str]]) -> typing.Iterable[str]:
    if isinstance(file_or_files_list, str):
        files = [file_or_files_list]
    else:
        files = list(file_or_files_list)
    return files
