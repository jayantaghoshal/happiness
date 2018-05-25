# LVDSLink tests

Prerequisites:
1. To be run on IHU hardware i.e. no mocking is done.
2. IHU hardware fs nodes in /sys and /dev must be present.
3. Assumes a MAX9288 deserializer.

## Gpio
Used gpios in LVDSLink are found in 24140544_VOLVO_IHU_MB_AL_HW02.0_SCHEMATIC_MID_DAB.pdf on page 14 for Apollo Lake SOC.
Detailed documentation for MAX9288 is in MAX9288-MAX9290_Rev2_11_2014.pdf.

| Schematics name         | Operation | Abbreviated description                                                               |
| SOC_GPIO28_LVDS_IN_PWD  | set/get   | Set low to enter power down mode to reduce power consumption                          |
| LVDS_IN_SOC_GPIO29_INT  | get       | A/V Status Register Interrupt Output. Indicates new data in the A/V status registers. |
| LVDS_IN_SOC_GPIO30_ERR  | get       | Error output                                                                          |
| LVDS_IN_SOC_GPIO31_LOCK | get       | Indicates PLL lock status                                                             |
| LVDS_IN_SOC_GPIO32_LFLT | get       | Line fault                                                                            |

The actual gpio setup is done by code in kernel/bxt/drivers/pinctrl/intel/pinctrl-broxton.c (apl_north_pins).
