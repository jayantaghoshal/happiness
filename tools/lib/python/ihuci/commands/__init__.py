# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from .builds import BuildsCommand
from .agent import AgentCommand

command_list = [
    BuildsCommand,
    AgentCommand,
]

