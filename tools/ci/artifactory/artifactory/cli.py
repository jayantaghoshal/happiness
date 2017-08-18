import argparse
import textwrap
import sys
import json
import os
from .artifactory import Artifactory

class ArgumentHandler(object):

    def parse(self):
        parser = argparse.ArgumentParser(
            prog='artifactory',
            formatter_class=argparse.RawDescriptionHelpFormatter,
            description=textwrap.dedent('''\
            Command line tool to interact with Artifactory

            Commands:
            push     - Upload files to Artifactory
            pull     - Download files from Artifactory
            show     - View attributes
            ''')
        )

        ##
        # Global options
        #
        parser.add_argument('-v', '--verbose',
                            action='store_true',
                            default=False,
                            help='verbose mode')

        subparsers = parser.add_subparsers()

        ##
        # Create the parser for the "push" command
        #
        push_parser = subparsers.add_parser('push',
                                            description='Upload files to Artifactory')

        push_parser.add_argument(
            'project',
            metavar='PROJECT-NAME',
            help='Jenkins project name'
        )

        push_parser.add_argument(
            'job',
            metavar='JOB-ID',
            type=str,
            help='Jenkins job ID or Zuul change IDs (ZUUL_CHANGE_IDS env var)'
        )

        push_parser.add_argument(
            'files',
            nargs='+',
            metavar='FILE',
            help='Files to upload'
        )

        push_parser.set_defaults(func=self.push)

        ##
        # Create the parser for the "pull" commnad
        #
        pull_parser = subparsers.add_parser('pull',
                                            description='Download files from Artifactory')

        pull_parser.add_argument(
            'project',
            metavar='PROJECT-NAME',
            help='Jenkins project name'
        )

        pull_parser.add_argument(
            'job',
            metavar='JOB-ID',
            type=str,
            help='Jenkins job ID or Zuul change IDs (ZUUL_CHANGE_IDS env var)'
        )

        pull_parser.add_argument(
            'files',
            nargs='*',
            metavar='FILE',
            help='Files to upload'
        )

        pull_parser.add_argument(
            '-d', '--dir',
            default=os.getcwd(),
            help='Directory path where to put the downloaded files'
        )

        pull_parser.set_defaults(func=self.pull)

        ##
        # Create the parser for the "show" commnad
        #
        show_parser = subparsers.add_parser('show',
                                            description='Show content in Artifactory')

        show_parser.add_argument(
            'project',
            metavar='PROJECT-NAME',
            nargs='?',
            help='Jenkins project name'
        )

        show_parser.add_argument(
            'job',
            metavar='JOB-ID',
            nargs='?',
            type=str,
            help='Jenkins job ID or Zuul change IDs (ZUUL_CHANGE_IDS env var)'
        )

        show_parser.add_argument(
            'file',
            nargs='?',
            metavar='FILE',
            help='File to show'
        )

        show_parser.set_defaults(func=self.show)

        return parser

    def push(self, args):
        artifactory = Artifactory()
        for file in args.files:
            path = "ICUP_ANDROID_CI/%s/%s/%s" % (args.project, args.job, os.path.basename(file))
            print("Deploy file to Artifactory - %s -> %s" % (file, path))
            artifactory.deploy_artifact(path, file)

    def pull(self, args):
        artifactory = Artifactory()
        for file in args.files:
            path = "ICUP_ANDROID_CI/%s/%s/%s" % (args.project, args.job, file)
            print("Retrieve file from Artifactory - %s -> %s/%s" % (path, args.dir, file))
            artifactory.retrieve_artifact(path, args.dir)

    def show(self, args):
        artifactory = Artifactory()
        path = "ICUP_ANDROID_CI"
        if args.project != None:
            path = path + "/" + args.project

        if args.job != None:
            path = path + "/" + str(args.job)

        if args.file != None:
            path = path + "/" + args.file

        response = artifactory.properties(path)
        if args.verbose:
           print(json.dumps(response.json(),
                            sort_keys=True,
                            indent=4,
                            separators=(',', ': ')))

        if args.file:
            print(json.dumps(response.json(),
                             sort_keys=True,
                             indent=4,
                             separators=(',', ': ')))
        else:
            for child in response.json()['children']:
                print(path + child['uri'])


def main():
    '''Main function'''
    parser = ArgumentHandler().parse()
    args = parser.parse_args()
    if hasattr(args, 'func'):
        args.func(args)
    else:
        parser.print_help()
        sys.exit(1)