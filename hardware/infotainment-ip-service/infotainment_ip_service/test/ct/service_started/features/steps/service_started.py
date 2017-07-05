from behave import *
import re
from test_tools.target import *
from test_tools.log import *

def search_in_log(search_expression, log_txt):
    escaped_search_expression = re.escape(search_expression)
    regex = re.compile(r'' + escaped_search_expression, re.I)
    result = regex.findall(log_txt)
    return result

@given('we have started the ihu')
def step_impl(context):
    # TODO: We will need some state handling here later
    #       when verifying shutdown too.
    pass

@then('check that InfotainmentIpService has started')
def step_impl(context):
    iips_log = execute_on_target("systemctl status infotainment-ip-service; exit 0")
    write_log(iips_log, "iips.log")
    print("Please see stored IIPS log artifact to analyze problems.")
    assert(search_in_log("running", iips_log))

