import os

ignored_source_files = ['Android.mk', 'Android.bp', '__init__.py', '.vcc-component.yaml']
ignored_directories = ['.git', '.idea', '__pycache__']


def collect_source_directories(path):
    contains_sources = False
    for entry in os.scandir(path):
        if entry.is_dir() and not entry.name in ignored_directories:
            yield from collect_source_directories(entry.path)

        if entry.is_file() and entry.name not in ignored_source_files:
            contains_sources = True

    if contains_sources:
        yield path


def build_source_directories_list(root: str):
    for path in collect_source_directories(root):
        relpath = os.path.relpath(path, root)
        if relpath is not '.':
            yield '/' + relpath
