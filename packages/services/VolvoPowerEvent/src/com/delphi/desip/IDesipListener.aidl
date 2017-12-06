// IDesipListener.aidl
package com.delphi.desip;

import com.volvocars.powerevent.ParcelableDesipMessage;

interface IDesipListener {

    boolean deliverMessage(in ParcelableDesipMessage msg);
}
