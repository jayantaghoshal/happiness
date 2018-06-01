# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import argparse
import textwrap
import tempfile
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
            push         - Upload files to Artifactory
            pull         - Download files from Artifactory
            pull-latest  - Download latest files or folder
            show         - View attributes
            meta-create  - add meta data to a artifactory
            meta-add     - add meta data to a file object
            meta-destroy - remove meta data file object
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

        push_parser.add_argument(
            '--metafile',
            metavar='METAFILE',
            type=str,
            default=None,
            help='text file containing meta data in JSON format'
        )

        push_parser.set_defaults(func=self.push)

        ##
        # Create the parser for the "pull" command
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
            help='Files to download'
        )

        pull_parser.add_argument(
            '-d', '--dir',
            default=os.getcwd(),
            help='Directory path where to put the downloaded files'
        )

        pull_parser.set_defaults(func=self.pull)


        ##
        # Create the parser for the "pull-latest" command
        #
        pull_latest_parser = subparsers.add_parser('pull-latest',
                                            description='Download latest files from Artifactory')

        pull_latest_parser.add_argument(
            'project',
            metavar='PROJECT-NAME',
            help='Jenkins project name to get the latest files or file'
        )

        pull_latest_parser.add_argument(
            'files',
            nargs='*',
            metavar='FILE',
            help='Files to download'
        )

        pull_latest_parser.add_argument(
            '-d', '--dir',
            default=os.getcwd(),
            help='Directory path where to put the downloaded files'
        )

        pull_latest_parser.set_defaults(func=self.pull_latest)


        ##
        # Create the parser for the "show" command
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

        ##
        # Create a parser for the "meta" command
        #
        metacreate_parser = subparsers.add_parser('meta-create',
                                            description='handle Artifactory properties')

        metaadd_parser = subparsers.add_parser('meta-add',
                                            description='handle Artifactory properties')

        metadel_parser = subparsers.add_parser('meta-destroy',
                                            description='handle Artifactory properties')

        metaadd_parser.add_argument(
            'metafile',
            metavar='FILE',
            action='store',
            help='the META file object, usually a file name'
        )

        metaadd_parser.add_argument(
            'metavar',
            metavar='VAR',
            action='store',
            help='unique name of the META variable'
        )

        metaadd_parser.add_argument(
            'metavalue',
            metavar='VALUE',
            action='store',
            help='content of the meta variable'
        )

        metadel_parser.add_argument(
            'metafile',
            metavar='FILE',
            action='store',
            help=''
        )

        metacreate_parser.set_defaults(func=self.metaCreate)
        metaadd_parser.set_defaults(func=self.metaAdd)
        metadel_parser.set_defaults(func=self.metaDestroy)

        return parser

    def push(self, args):
        artifactory = Artifactory()
        for file in args.files:
            uri = "ICUP_ANDROID_CI/%s/%s/%s" % (args.project, args.job, os.path.basename(file))
            artifactory.deploy_artifact(uri, file)
            if args.metafile:
                artifactory.set_properties(uri, args.metafile)

    def pull_latest(self, args):
        artifactory = Artifactory()
        uri = "ICUP_ANDROID_CI/%s" % (args.project)
        latest_job = artifactory.retreive_latest_uri(uri)
        if not args.files:
            artifactory.retrieve_artifacts(latest_job, args.dir)
        else:
            for file in args.files:
                uri = latest_job + "/%s" % (file)
                artifactory.retrieve_artifact(uri, args.dir)

    def pull(self, args):
        artifactory = Artifactory()
        if not args.files:
            uri = "ICUP_ANDROID_CI/%s/%s" % (args.project, args.job)
            artifactory.retrieve_artifacts(uri, args.dir)
        else:
            for file in args.files:
                uri = "ICUP_ANDROID_CI/%s/%s/%s" % (args.project, args.job, file)
                artifactory.retrieve_artifact(uri, args.dir)

    def metaCreate(self, args):
        _name = Artifactory.meta_create()
        print(_name)
        return 0

    def metaAdd(self, args):
        if not Artifactory.meta_add(args.metafile, args.metavar, args.metavalue):
            print('Could not find temporary file object storing meta data')
            return 255

        return 0

    def metaDestroy(self, args):
        _name = Artifactory.meta_destroy(os.path.abspath(args.metafile))
        return 0

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
        sys.exit(args.func(args))
    else:
        parser.print_help()
        sys.exit(1)
