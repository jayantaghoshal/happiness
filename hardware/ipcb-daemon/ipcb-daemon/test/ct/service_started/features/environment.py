from test_tools.target import *

def after_all(context):
    # Get all available logs and store them in a zip file.
    print("Saving all logs from target...")
    # TODO: We sleep now as we don't wait for the sync when the dump_log has finished.
    log_file_name = "dump-logs.tar.gz"
    log_directory = "dump-logs"
    execute_on_target("rm -rf " + log_directory +
                      " && mkdir -p " + log_directory +
                      " && touch " + log_directory + "/dump-logs.ihu " +
                      " && touch " + log_directory + "/dump-cores.ihu " +
                      " && dump_log.sh /home/root/" + log_directory +
                      " && sleep 10 " +
                      " && tar -cvzf " + log_file_name + " " + log_directory)
    copy_file_from_target(log_file_name, "./logs")
