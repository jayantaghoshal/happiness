# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import datetime
from collections import namedtuple

import os
import re

LanguageCommentFeatures = namedtuple('LanguageCommentFeatures', ['firstline',
                                                                 'endline',
                                                                 'bodyline_prefix',
                                                                 'bodyline_suffix',
                                                                 'headers_matcher',
                                                                 'headers_to_replace',
                                                                 'supports_shebang'])

CStyleGenericRegex = re.compile(r'(^/\*.*Copyright.*?\*\/)',
                                re.IGNORECASE)
CStyleReplaceableRegex = re.compile(r'(^/\*[\s\S]*(?:(Copyright)|(\[2017\])).*(?:(?:Delphi)|(?:Volvo))[\s\S]*?\*\/)',
                                    re.IGNORECASE)

CLangFeatures = LanguageCommentFeatures(firstline='/*\n',
                                        endline=' */\n\n',
                                        bodyline_prefix=' * ',
                                        bodyline_suffix='\n',
                                        headers_matcher=[CStyleGenericRegex],
                                        headers_to_replace=[CStyleReplaceableRegex],
                                        supports_shebang=False
                                        )

HashStartGenericRegex = re.compile(r'(^#.*Copyright.*?\n)')
HashStartReplaceableRegex = re.compile(r'(^#.*(?:Copyright.*(?:(?:Delphi)|(?:Volvo)).*\n)(.*\n)*?\n)',
                                       re.IGNORECASE)
HashStartLanguageFeatures = LanguageCommentFeatures(firstline='',
                                                    endline='\n',
                                                    bodyline_prefix='# ',
                                                    bodyline_suffix='\n',
                                                    headers_matcher=[HashStartGenericRegex],
                                                    headers_to_replace=[HashStartReplaceableRegex],
                                                    supports_shebang=False
                                                    )

HashStartLanguageWithShebangFeatures = HashStartLanguageFeatures._replace(supports_shebang=True)

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
}

PathsIgnoreRegex = re.compile(r'.*(?:'
                              r'(generated)|'
                              r'(gen)|'
                              r'(__init__.py)|'
                              r'(vendor/volvocars/interfaces/.*\.(?:bp|mk))'
                              r')')


def should_filepath_be_ignored(path: str):
    abspath = os.path.abspath(path)
    return PathsIgnoreRegex.match(abspath) is not None


def filter_ignored(files):
    for f in files:
        if not should_filepath_be_ignored(f):
            yield f
        else:
            print("Ignoring file {}".format(f))


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


def sanitize_whitespace(text):
    trailing_strip_re = re.compile(r'[ \t]+(\n|\Z)')
    text = trailing_strip_re.sub(r"\1", text).lstrip('\r\n ').replace('\r', '')
    return text


def get_lang(filename: str):
    ext = os.path.splitext(filename)[1]
    if ext not in KnownLanguages:
        raise Exception("File {0} is of not supported language {1}".format(filename, ext))

    lang = KnownLanguages.get(ext)
    return lang


def __remove_header_from_body(file_body: str, lang: LanguageCommentFeatures):
    assert isinstance(lang, LanguageCommentFeatures)

    file_body_without_headers = file_body
    for regex in lang.headers_to_replace:
        file_body_without_headers = regex.sub('', file_body_without_headers)

    file_body_without_headers = sanitize_whitespace(file_body_without_headers)

    return file_body_without_headers


def __add_header_to_body(file_body: str, lang: LanguageCommentFeatures):
    copyright_header = lang.firstline

    for line in CommentPattern.splitlines(keepends=False):
        copyright_header += lang.bodyline_prefix
        copyright_header += line.replace('{year}', str(datetime.datetime.now().year))
        copyright_header += lang.bodyline_suffix

    copyright_header += lang.endline

    header_after_removal_processing = __remove_header_from_body(copyright_header, lang)
    if header_after_removal_processing != "":
        raise Exception("Internal error, generated header would not be removable")

    return copyright_header + file_body


def __replace_headers_in_body(file_body: str, lang: LanguageCommentFeatures):
    shebang_statement = None

    # extract the shebang from the top of the file
    if lang.supports_shebang and file_body.startswith("#!"):
        shebang_statement, file_body = file_body.split('\n', 1)
        # removing of trailing whitespace needed for reproducibility,
        file_body = sanitize_whitespace(file_body)

    decopyrighted = __remove_header_from_body(file_body, lang)

    # recover the shebang if the header was only thing above it
    if lang.supports_shebang and decopyrighted.startswith("#!"):
        shebang_statement, decopyrighted = decopyrighted.split('\n', 1)
        # removing of trailing whitespace needed for reproducibility,
        decopyrighted = sanitize_whitespace(decopyrighted)

    with_copyright_applied = __add_header_to_body(decopyrighted, lang)

    if shebang_statement is not None:
        with_copyright_applied = shebang_statement + "\n\n" + with_copyright_applied

    return with_copyright_applied


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
