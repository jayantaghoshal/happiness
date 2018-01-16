# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import jinja2
import os
from licensingcomponents.softwarebom import descriptions, source, validation

env = jinja2.Environment(
    loader=jinja2.PackageLoader('licensingcomponents'),
    autoescape=jinja2.select_autoescape(['html', 'xml'])
)


def software_bom(root, output_directory):
    components_paths = list(descriptions.find_vcc_components_descriptions(root))
    components_infos = [descriptions.load_and_validate(p, root) for p in components_paths]

    source_directories = list(source.build_source_directories_list(root))

    validation_result = validation.build_and_validate_licensing_info(components_infos, source_directories)

    os.makedirs(output_directory, exist_ok=True)
    bom_html_file_path = os.path.join(output_directory, 'software-bom.html')

    with open(bom_html_file_path, 'w', encoding="utf-8") as file:
        template = env.get_template('software-bom.html')
        file.writelines(template.render(components=validation_result.components,
                                        components_with_missing_info=validation_result.underspecified_components,
                                        sources=source_directories,
                                        not_accounted_sources=validation_result.not_accounted_sources
                                        ))
