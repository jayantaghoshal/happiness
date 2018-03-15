# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

vcc_software_bom_report := $(OUT_DIR)/vcc-software-bom.html

$(call dist-for-goals,droidcore,$(vcc_software_bom_report):$(notdir $(software_bom_report)))

$(vcc_software_bom_report):
		licensing-components.py software-bom vendor/volvocars/ $(OUT_DIR)

vcc_software_bom_report: $(vcc_software_bom_report)

include $(call all-subdir-makefiles)
