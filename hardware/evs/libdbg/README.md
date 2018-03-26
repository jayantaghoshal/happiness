
The system property debug.evs.loglevel is used to filter debug prints depending on the value of the argument.

syntax: setprop debug.evs.loglevel <loglevel>
 loglevel values = [ verbose < debug < info < warn < error < fatal ]

The property permits debug prints that are greater than or equal to <loglevel> value.