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
            nodes   - List Jenkins nodes
            vbf     - IHU VBF management
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

    ##
    # nodes
    #
    parser_nodes = subparser.add_parser("nodes",
                                        description="List nodes in Jenkins")

    parser_nodes.add_argument("-u", "--user",
                              nargs="?",
                              dest="username",
                              metavar='USER',
                              default=os.getenv('USER'),
                              help="CDSID (defaults to $USER)")

    parser_nodes.add_argument("-p", "--password",
                              nargs="?",
                              metavar='API-KEY',
                              default=os.getenv('JENKINS_API_KEY', None),
                              help="Jenkins API-KEY (defaults to ${JENKINS_API_KEY})")

    ##
    # vbf
    #
    parser_vbf = subparser.add_parser("vbf",
                                      description="IHU VBF management")

    vbf_subparser = parser_vbf.add_subparsers(dest="action")

    ##
    # GET
    #
    parser_vbf_get = vbf_subparser.add_parser("get",
                                              description="Get VBF part number and version")

    parser_vbf_get.add_argument("sw_tag",
                                metavar='SW-TAG',
                                help="SW Tag (e.g. SWL2)")

    parser_vbf_get.add_argument("-t", "--build-type",
                                nargs="?",
                                metavar='BUILD-TYPE',
                                default="eng",
                                help="AOSP Build Type (defaults to \"eng\"")

    ##
    # INC
    #
    parser_vbf_inc = vbf_subparser.add_parser("inc",
                                              description="Increment VBF version")

    parser_vbf_inc.add_argument("sw_tag",
                                metavar='SW-TAG',
                                help="SW Tag (e.g. SWL2)")

    parser_vbf_inc.add_argument("-t", "--build-type",
                                nargs="?",
                                metavar='BUILD-TYPE',
                                default="eng",
                                help="AOSP Build Type (defaults to \"eng\"")
    ##
    # SET
    #
    parser_vbf_set = vbf_subparser.add_parser("set",
                                              description="Set VBF part number and version")

    parser_vbf_set.add_argument("sw_tag",
                                metavar='SW-TAG',
                                help="SW Tag (e.g. SWL2)")

    parser_vbf_set.add_argument("part_number",
                                metavar='PART-NUMBER',
                                help="VBF Partnumber")

    parser_vbf_set.add_argument("suffix",
                                metavar='SUFFIX',
                                help="VBF suffix (version)")

    parser_vbf_set.add_argument("-t", "--build-type",
                                nargs="?",
                                metavar='BUILD-TYPE',
                                default="eng",
                                help="AOSP Build Type (defaults to \"eng\"")

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
    elif options.command == 'nodes':
        return NodesCommand(options).run()
    elif options.command == 'vbf':
        return VBFCommand(options).run()

def main(argv=None):

    if argv is None:
        argv = sys.argv[1:]

    parser = create_parser()
    options = parser.parse_args(argv)
    create_logger(options)
    if not options.command:
        parser.error("Must specify a 'command' to be performed")
    if options.command == "vbf":
        if not options.action:
           parser.error("Must specify an 'action' (set,get,inc) to be performed")

    return execute(options)
