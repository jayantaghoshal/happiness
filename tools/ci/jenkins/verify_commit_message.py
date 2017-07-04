import subprocess
import re
import sys
repo_path = sys.argv[1]
commit_message = subprocess.check_output(["git", "log", "--format=%B", "-n", "1", "HEAD"],
                                         cwd=repo_path).decode()


pattern = r"\[((PSS370-\d+)|(No-Jira))\]"
matches = re.search(pattern, commit_message)
if matches is None:
    print("--------------------------------------------------------------------------------")
    print(" INVALID COMMIT MESSAGE")
    print(" ")
    print(" Your commit message must refer to a JIRA issue id ")
    print(" enclosed with square brackets, eg [PSS370-000]")
    print(" Add this to the end of your commit message")
    print("")
    print(" If your change does not have an issue id, add [No-Jira] to the end of your commit message.")
    print("--------------------------------------------------------------------------------")
    sys.exit(-1)