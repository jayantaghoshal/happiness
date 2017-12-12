// IDesip.aidl
package com.delphi.desip;

import com.delphi.desip.IDesipListener;
import com.volvocars.powerevent.ParcelableDesipMessage;

interface IDesip {

    boolean addListener(in String uid, in IDesipListener listener, in byte[] appIds);
    boolean removeListener(in String uid);
    boolean send(in ParcelableDesipMessage msg);
}
