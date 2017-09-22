set(SPEC carconfigspec.xls)
set(TOOLS ../tools)
set(PARSER ${TOOLS}/parse_ccdb.py)

set(CEDRIC_TEMPLATE ${TOOLS}/ccdb_template.h)
set(CEDRIC_OUTPUT ../include/cedric_parameterlist.h)

set(UPDATER_TEMPLATE ${TOOLS}/ccdb_paramlist_template.h)
set(UPDATER_OUTPUT ../../carconfig_updater/include/carconfig_parameterlist.h)

add_custom_target(generate-cedric-header ALL
                  python ${PARSER} ${SPEC} ${CEDRIC_TEMPLATE} ${CEDRIC_OUTPUT}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  DEPENDS ${PARSER} ${SPEC} ${CEDRIC_TEMPLATE})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../carconfig_updater/include)
add_custom_target(generate-updater-header ALL
                  python ${PARSER} ${SPEC} ${UPDATER_TEMPLATE} ${UPDATER_OUTPUT}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  DEPENDS ${PARSER} ${SPEC} ${UPDATER_TEMPLATE})
