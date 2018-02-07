# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from collections import namedtuple
from typing import List

from licensingcomponents.swbom.descriptions import ComponentInfo

ComponentSources = namedtuple('ComponentSources', ['info',
                                                   'directories',
                                                   'exclusions'])

ValidationResult = namedtuple('ValidationResult', ['components',
                                                   'not_accounted_sources',
                                                   'underspecified_components'])


def component_is_underspecified(cmp: ComponentInfo):
    if cmp.description.component == "TBD":
        return True
    if cmp.description.functional_area == "TBD":
        return True
    return False


def build_and_validate_licensing_info(components_infos: List[ComponentInfo],
                                      directories_with_source: List[str]) -> ValidationResult:
    components_by_nesting = sorted(components_infos, key=lambda info: len(info.path))  # type List[ComponentInfo]
    component_sources = [ComponentSources(info, [], []) for info in
                         components_by_nesting]  # type List[ComponentSources]

    not_accounted_source_dirs = list()

    for dws in directories_with_source:
        dws_accounted_for = False
        for cbs in component_sources:
            if dws.startswith(cbs.info.path):
                cbs.directories.append(dws)
                dws_accounted_for = True
                break
        if not dws_accounted_for:
            not_accounted_source_dirs.append(dws)

    for cbs in component_sources:
        cbs.directories.sort()

    for cmp in component_sources:
        for subcmp in component_sources:
            cmp_path = cmp.info.path
            subcmp_path = subcmp.info.path
            if subcmp_path.startswith(cmp_path) and cmp_path != subcmp_path:
                cmp.exclusions.append(subcmp_path)

    underspecified_components = [cmp for cmp in component_sources if component_is_underspecified(cmp.info)]

    return ValidationResult(component_sources, not_accounted_source_dirs, underspecified_components)
