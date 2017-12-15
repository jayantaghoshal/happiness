# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""
licensing-components

Usage:
  licensing-components software-bom <root> <output_path>
  licensing-components list-copyright-headers <files>...
  licensing-components fix-copyright-headers <files>...
  licensing-components fix-all-copyright-headers <directory>
  licensing-components verify-copyright-headers <files>...
  licensing-components -h | --help
  licensing-components --version

Options:
  -h --help                         Show this screen.
  --version                         Show version.

Examples:
  licensing-components software-bom /directory

Help:
  This tool support tracking of component organization and listing for
  Volvo Car Corporation developed code, shared with partners.

  It gathers data from .vcc-component.yaml files, and uses it to label
  the files, create software bom etc.
"""
import os

from jinja2 import Environment, PackageLoader, select_autoescape

from licensingcomponents.common import descriptions, source, validation
from licensingcomponents.copyrightheader.copyrightheader import filter_ignored, should_filepath_be_ignored
from licensingcomponents.docopt_dispatch import dispatch
from licensingcomponents.copyrightheader import copyrightheader

env = Environment(
    loader=PackageLoader('licensingcomponents'),
    autoescape=select_autoescape(['html', 'xml'])
)


@dispatch.on('software-bom')
def software_bom(root, output_path, **kwargs):
    components_paths = list(descriptions.find_vcc_components_descriptions(root))
    components_infos = [descriptions.load_and_validate(p, root) for p in components_paths]

    source_directories = list(source.build_source_directories_list(root))

    validation_result = validation.build_and_validate_licensing_info(components_infos, source_directories)

    os.makedirs(output_path, exist_ok=True)
    bom_html_file_path = os.path.join(output_path, 'software-bom.html')

    with open(bom_html_file_path, 'w', encoding="utf-8") as file:
        template = env.get_template('software-bom.html')
        file.writelines(template.render(components=validation_result.components,
                                        components_with_missing_info=validation_result.underspecified_components,
                                        sources=source_directories,
                                        not_accounted_sources=validation_result.not_accounted_sources
                                        ))


@dispatch.on('list-copyright-headers')
def list_copyright_headers(files, **kwargs):
    for f in files:
        if should_filepath_be_ignored(f):
            print("Ignoring file {}".format(f))
            continue

        copyrightheader.list_headers_and_filenames(f)
    pass


@dispatch.on('fix-copyright-headers')
def fix_copyright_headers(file_or_files_list, **kwargs):
    if isinstance(file_or_files_list, str):
        files = [file_or_files_list]
    else:
        files = file_or_files_list

    for f in filter_ignored(files):
        assert os.path.isfile(f)

        fixed, original = copyrightheader.get_contents_with_header_applied(f)
        if fixed != original:
            with(open(f, 'w', encoding="utf-8")) as target:
                target.write(fixed)
            print("Fixed header in file {0}".format(f))
    pass


def collect_supported_files(directory):
    for entry in os.scandir(directory):
        if entry.is_dir():
            yield from collect_supported_files(entry.path)

        if entry.is_file() and os.path.splitext(entry.path)[1] in copyrightheader.get_known_languages_extensions():
            yield entry.path
    pass


def collect_copyrightable_files(directory):
    yield from filter_ignored(collect_supported_files(directory))


@dispatch.on('fix-all-copyright-headers')
def fix_all_copyright_headers(directory, **kwargs):
    for path in collect_copyrightable_files(directory):
        fix_copyright_headers(path)


@dispatch.on('verify-copyright-headers')
def verify_copyright_headers(files, **kwargs):
    for f in filter_ignored(files):

        fixed, original = copyrightheader.get_contents_with_header_applied(f)
        if fixed != original:
            raise Exception("Copyright header invalid in file {0}".format(f))
    pass


if __name__ == "__main__" and __package__ is None:
    dispatch(__doc__)
