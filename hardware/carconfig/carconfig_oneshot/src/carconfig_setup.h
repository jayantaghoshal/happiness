/*===========================================================================*\
 * Copyright 2016, 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

// Copies the default file to final destination if it is not there yet
// and uses CarConfigWriter to put it into shared memory.
int carconfig_setup(const char* ccFileName, const char* ccDefaultFileName);
