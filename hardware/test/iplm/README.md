# Introduction
This document describes the integration test bench for IplmD.

## Unit under test
This test tests IplmD together with IpcbD

## Test harness
For this test mocks for the following interfaces are used

### Ipcb
Packet injector is used to inject and read packets sent over IP Command Bus from ipcbD

### IIplm.hal
For the IIplm HAL exposed by IplmD a mock is written that attaches to the HAL to be able to verify functionality of the IIplm HAL interface

### Flexray
A Flexray sink is used to verify that IplmD send the correct signals and react correctly to signals being sent to it.