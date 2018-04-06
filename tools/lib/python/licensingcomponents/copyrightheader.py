# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import typing
import collections
import os
import re
from datetime import datetime
from ihuutils.sourcecode import sanitize_whitespace


class LicenseHeaderInvalidError(Exception):
    def __init__(self, violation, source_file):
        # Call the base class constructor with the parameters it needs
        super(LicenseHeaderInvalidError, self).__init__("{} in file {}".format(violation, source_file))


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
CStyleGenericRegex = re.compile(r'^\s\*\s(Copyright.*)',
                                re.IGNORECASE | re.MULTILINE)
CStyleReplaceableRegex = re.compile(r'(^/\*\n\s\*\sCopyright.*(Delphi|Volvo)(.*\n)*?\s\*/)',
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
                                re.IGNORECASE);
HashStartGenericRegex = re.compile(r'^#\s(Copyright.*)',
                                   re.IGNORECASE | re.MULTILINE)
HashStartReplaceableRegex = re.compile(r'(^#\s(Copyright.*?((Delphi)|(Volvo)).*?\n)(#.*?\n)+)',
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
                              r'(__init__.py)|'
                              r'(vendor/volvocars/interfaces/.*\.(?:bp|mk))'
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


def list_headers_and_filenames(filename: str):
    with open(filename, 'r', encoding="utf-8") as file:
        file_body = file.read()

    headers = []
    for regex in get_lang(filename).headers_matcher:
        for m in regex.findall(file_body):
            headers.append(m)

    if headers:
        print(filename)
        for h in headers:
            print(h)


def get_lang(filename: str):
    ext = os.path.splitext(filename)[1]
    if ext not in KnownLanguages:
        raise Exception("File {0} is of not supported language {1}".format(filename, ext))

    lang = KnownLanguages.get(ext)
    return lang


def __get_copyright_info_from_body(file_body: str, lang: LanguageCommentFeatures):
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
            break;

    if match is None:
        return None

    return CopyrightInfo(start_year=match.group('start_year'),
                         end_year=match.group('end_year'),
                         company=match.group('company'))


def __remove_header_from_body(file_body: str, lang: LanguageCommentFeatures):
    assert isinstance(lang, LanguageCommentFeatures)

    file_body_without_headers = file_body
    for regex in lang.headers_to_replace:
        file_body_without_headers = regex.sub('', file_body_without_headers)

    file_body_without_headers = sanitize_whitespace(file_body_without_headers)

    return file_body_without_headers


def __add_header_to_body(file_body: str, copyright_year: str, lang: LanguageCommentFeatures):
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


def __replace_headers_in_body(file_body: str, lang: LanguageCommentFeatures):
    removed_headers, filtered_file_body = __filter_lines_before_copyright(file_body, lang)

    # Get year and company information from the current Copyright header if there is one
    info = __get_copyright_info_from_body(filtered_file_body, lang)
    current_year = datetime.now().year
    copyright_year = None

    if info is not None:
        if info.company == "Delphi":
            # If the company is Delphi a new copyright header shall be written
            copyright_year = str(current_year)
        elif current_year > int(info.start_year):
            # If the Copyright header contains an older year the header shall be written with a range from then to now
            copyright_year = "{0}-{1}".format(info.start_year, str(current_year))

    # If none of the above clauses apply write the current year in the Copyright header
    if copyright_year is None:
        copyright_year = str(current_year)

    # removing of trailing whitespace needed for reproducibility,
    filtered_file_body = sanitize_whitespace(filtered_file_body)
    decopyrighted = __remove_header_from_body(filtered_file_body, lang)
    # removing of trailing whitespace needed for reproducibility,
    decopyrighted = sanitize_whitespace(decopyrighted)

    with_copyright_applied = __add_header_to_body(decopyrighted, copyright_year, lang)
    if len(removed_headers) > 0:
        removed_headers.append("\n")
    with_copyright_and_saved_lines_applied = "".join(removed_headers + [with_copyright_applied])
    return with_copyright_and_saved_lines_applied


def get_contents_with_header_applied(filename: str):
    assert isinstance(filename, str)
    filename = os.path.abspath(filename)

    lang = get_lang(filename)

    with open(filename, 'r', encoding="utf-8") as file:
        file_body = file.read()

    with_copyright_applied = __replace_headers_in_body(file_body, lang)
    with_copyright_applied_twice = __replace_headers_in_body(with_copyright_applied, lang)

    if with_copyright_applied_twice != with_copyright_applied:
        with open(filename + ".iter_orig", "w") as copy_not_applied:
            copy_not_applied.write(file_body)

        with open(filename + ".iter1", "w") as copy_applied_once:
            copy_applied_once.write(with_copyright_applied)

        with open(filename + ".iter2", "w") as copy_applied_twice:
            copy_applied_twice.write(with_copyright_applied_twice)

        raise Exception("Internal error, non reproducible application of header to file {}".format(filename))

    return with_copyright_applied, file_body


def verify_copyright_in_file(filename: str):
    """ Verify the Copyright header in the specified file

    Args:
        filename: The file to verify

    Raises:
        LicenseHeaderInvalidError: If the Copyright header is invalid
    """
    assert isinstance(filename, str)
    file_path = os.path.abspath(filename)

    with open(file_path, 'r', encoding="utf-8") as file:
        file_body = file.read()

    lang = get_lang(file_path)

    _, filtered_file_body = __filter_lines_before_copyright(file_body, lang)
    filtered_file_body = sanitize_whitespace(filtered_file_body)
    info = __get_copyright_info_from_body(filtered_file_body, lang)
    current_year = datetime.now().year

    if info is None:
        # Copyright head has unknown format
        raise LicenseHeaderInvalidError("Copyright header invalid", filename)
    elif int(info.start_year) < 2017:
        # The old script was hard coded to only accept 2017, so copyright before that is not allowed
        raise LicenseHeaderInvalidError("Copyright header invalid year", filename)
    elif info.end_year is not None:
        # If the Copyright header contains a year range verify that the range is valid
        if int(info.end_year) <= int(info.start_year) or current_year < int(info.end_year):
            raise LicenseHeaderInvalidError("Copyright header invalid year range", filename)


def fix_copyright_headers(file_or_files_list: typing.Union[str, typing.Iterable[str]]):
    if isinstance(file_or_files_list, str):
        files = [file_or_files_list]
    else:
        files = list(file_or_files_list)

    for f in filter_ignored(files):
        assert os.path.isfile(f)

        fixed, original = get_contents_with_header_applied(f)
        if fixed != original:
            with(open(f, 'w', encoding="utf-8")) as target:
                target.write(fixed)
            print("Fixed header in file {0}".format(f))
    pass


def verify_copyright_headers(file_or_files_list: typing.Union[str, typing.Iterable[str]]):
    if isinstance(file_or_files_list, str):
        files = [file_or_files_list]
    else:
        files = list(file_or_files_list)

    for f in filter_ignored(files):
        verify_copyright_in_file(f)
    pass


def collect_supported_files(directory):
    for entry in os.scandir(directory):
        if entry.is_dir():
            yield from collect_supported_files(entry.path)

        if entry.is_file():
            filename, ext = os.path.splitext(entry.path)
            if ext in get_known_languages_extensions():
                yield entry.path
    pass


def collect_copyrightable_files(directory):
    yield from filter_ignored(collect_supported_files(directory))


def list_copyright_headers(file_or_files_list):
    if isinstance(file_or_files_list, str):
        files = [file_or_files_list]
    else:
        files = file_or_files_list

    for f in files:
        if should_filepath_be_ignored(f):
            print("Ignoring file {}".format(f))
            continue

        list_headers_and_filenames(f)
    pass


def fix_all_copyright_headers(directory):
    for path in collect_copyrightable_files(directory):
        fix_copyright_headers(path)
