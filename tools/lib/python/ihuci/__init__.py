# -*- coding: utf-8 -*-
import argparse
import sys
import os
import logging
import textwrap
from .commands import * # FIXME

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger()

def create_parser():
    parser = argparse.ArgumentParser(
        prog='ihuci',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=textwrap.dedent('''
            Command line tool for IHU CI

            Commands:
            agent   - Execute Jenkins build
            builds  - List Jenkins builds
            ''')
    )

    ##
    # Global options
    #
    parser.add_argument('-d', '--debug',
                        action='store_true',
                        default=False,
                        help='debug mode')

    subparser = parser.add_subparsers(dest='command')

    ##
    # agent
    #
    parser_agent = subparser.add_parser("agent",
                                        description="Start the agent executing the Jenkins job")

    ##
    # builds
    #
    parser_builds = subparser.add_parser("builds",
                                         description="List builds in Jenkins")

    parser_builds.add_argument("job",
                               nargs="?",
                               metavar='JOB',
                               help="Jenkins job")

    parser_builds.add_argument("build",
                               nargs="?",
                               metavar='BUILD',
                               type=int,
                               help="Jenkins build number")

    parser_builds.add_argument("-m", "--max",
                               nargs="?",
                               dest="max",
                               metavar='MAX-BUILDS',
                               type=int,
                               default=30,
                               help="Max number of builds to list (defaults to 30)")

    parser_builds.add_argument("-u", "--user",
                               nargs="?",
                               dest="username",
                               metavar='USER',
                               default=os.getenv('USER'),
                               help="CDSID (defaults to $USER)")

    parser_builds.add_argument("-p", "--password",
                               nargs="?",
                               metavar='API-KEY',
                               default=os.getenv('JENKINS_API_KEY', None),
                               help="Jenkins API-KEY (defaults to ${JENKINS_API_KEY})")

    return parser

def create_logger(options):
    # TODO: Implement logging (formatter should be dependent on command)
    log = logging.getLogger('ihuci')
    log.setLevel(logging.INFO)

    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.INFO)
    if options.debug:
        console_handler.setLevel(logging.DEBUG)

    formatter = logging.Formatter('[%(asctime)s] %(message)s',
                                    datefmt='%Y-%m-%dT%H:%M:%S')
    console_handler.setFormatter(formatter)

    log.addHandler(console_handler)
    return log

def execute(options):
    logger = logging.getLogger('ihuci')
    if options.command == 'agent':
        return AgentCommand(options).run()
    elif options.command == 'builds':
        return BuildsCommand(options).run()

def main(argv=None):

    if argv is None:
        argv = sys.argv[1:]

    parser = create_parser()
    options = parser.parse_args(argv)
    create_logger(options)
    if not options.command:
        parser.error("Must specify a 'command' to be performed")

    return execute(options)
