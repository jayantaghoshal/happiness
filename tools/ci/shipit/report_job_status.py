import sys
from shipit import nightly_reporter


# Used to report hourly/nightly test status from Jenkins

def main():
    jenkins_job_name = sys.argv[1]
    jenkins_build_number = sys.argv[2]
    status = sys.argv[3].lower() #fail/pass

    if status == "pass":
        nightly_reporter.report_build_success(jenkins_job_name, jenkins_build_number)
    else:
        nightly_reporter.report_build_failed(jenkins_job_name, jenkins_build_number)


if __name__ == "__main__":
    main()
