def write_log(log_content, filename):
    file = open("logs/" + filename, "w")
    file.write(log_content)
    file.close()
