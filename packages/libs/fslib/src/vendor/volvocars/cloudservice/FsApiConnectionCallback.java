package vendor.volvocars.cloudservice;
/**
* For clients to implement to get callbacks
*/
public interface FsApiConnectionCallback {
   void onServiceConnected();
   void onServiceDisconnected();
}