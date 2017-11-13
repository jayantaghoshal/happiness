"""
licensing-components

Usage:
  licensing-components software-bom <root> <output_path>
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

  If we need to create file headers it will also automate this part.
"""
import os

from jinja2 import Environment, PackageLoader, select_autoescape

from licensingcomponents.common import descriptions, source, validation
from licensingcomponents.docopt_dispatch import dispatch

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


if __name__ == "__main__" and __package__ is None:
    dispatch(__doc__)
