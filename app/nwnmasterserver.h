#ifndef NWNMASTERSERVER_H
#define NWNMASTERSERVER_H

// [addicted2rpg] This is useful for redirecting the client to a dummmy master server, although no function is
// performed as of present (that I know of), although the connect succeeds and has a similar effect as the
// disablems plugin.  Currently unused.
#define NEW_MASTER "nwn1.mst.valhallalegends.com"
#define API_ENDPOINT "http://api.mst.valhallalegends.com/NWNMasterServerAPI/NWNMasterServerAPI.svc/ASMX"

#ifndef soapWSHttpBinding_USCOREINWNMasterServerAPIProxy_H
#include "soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h"
// Turn gSOAP generated names into more human friendly names.
typedef WSHttpBinding_USCOREINWNMasterServerAPIProxy NWNMasterServerAPIProxy;
typedef _ns1__GetOnlineServerList GetOnlineServerList;
typedef _ns1__GetOnlineServerListResponse GetOnlineServerListResponse;
typedef ns4__ArrayOfNWGameServer ArrayOfNWGameServer;
typedef ns4__NWGameServer NWGameServer;
typedef _ns1__LookupServerByGameType GameLookup;
typedef _ns1__LookupServerByGameTypeResponse GameLookupResponse;
typedef _ns1__RegisterPendingServers RegisterPendingServers;
typedef _ns1__RegisterPendingServersResponse RegisterPendingServersResponse;

#endif

int RoomToSkywing(int room);
const char * GetSoapErrorMessage(int res);
unsigned int AddServer(const char* address);

#endif // NWNMASTERSERVER_H
