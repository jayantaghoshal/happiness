import json
from collections import namedtuple

import glob
import os
import yaml

allowed_deployments = set([
    'none',
    'apk',
    'native-binary',
    'native-shared-lib',
    'native-static-lib',
    'native-suite',
    'src-gen',
    'host',
    'complete-image'
])

allowed_licenses = set(['Proprietary'])

ComponentDescription = namedtuple('ComponentDescription',
                                  ['functional_area',
                                   'component',
                                   'deployment',
                                   'license'])

ComponentInfo = namedtuple('ComponentInfo', ['description', 'path', 'absolute_path'])


def find_vcc_components_descriptions(root: str):
    directory = os.path.abspath(root)
    glob_pattern = os.path.join(directory, '**/.vcc-component.yaml')
    for filename in glob.iglob(glob_pattern, recursive=True):
        yield filename


def validate_description(description: ComponentDescription, path: str):
    deployments_set = set(description.deployment)
    if not deployments_set.issubset(allowed_deployments):
        raise Exception(
            "Component at {} has unknown deployment model: {},\n"
            "allowed deployments are defined in {}\nand they include:\n{}"
                .format(path, description.deployment, __file__, json.dumps(sorted(allowed_deployments), indent=4)))

    if description.license not in allowed_licenses:
        raise Exception(
            "Component at {} has unknown deployment model ({}),\n"
            "allowed deployments are defined in {}\nand they include:\n{}"
                .format(path, description.license, __file__, json.dumps(sorted(allowed_licenses), indent=4)))


def load_and_validate(path: str, root: str):
    assert os.path.isabs(path)
    with open(path, 'r', encoding='utf-8') as stream:
        loaded = yaml.load(stream)

        if type(loaded['deployment']) is not list:
            loaded['deployment'] = [loaded['deployment']]

        description = ComponentDescription(**loaded)
        validate_description(description, path)
        relative_path = "/" + os.path.relpath(os.path.dirname(path), root)
        info = ComponentInfo(description, relative_path, os.path.abspath(path))

    return info
