/*
 * Copied from Virusman & addicted2rpg NWNCX plugin.
 */

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif
#pragma comment(lib, "Ws2_32.lib")

#include <QString>
#include "nwnmasterserver.h"
#include "soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h"
#include "soap/WSHttpBinding_USCOREINWNMasterServerAPI.nsmap"

// The room list
// Action       nRoom=274   *(c->NWGameServer[i]->GameType) = 13;
// Roleplay     nRoom=275   *(c->NWGameServer[i]->GameType) = 3;
// Team         nRoom=276   *(c->NWGameServer[i]->GameType) = 4;
// Social       nRoom=277   *(c->NWGameServer[i]->GameType) = 7;
// PW Action    nRoom=278   *(c->NWGameServer[i]->GameType) = 9;
// Alternative  nRoom=279   *(c->NWGameServer[i]->GameType) = 8;
// Story        nRoom=363   *(c->NWGameServer[i]->GameType) = 1;
// Story Lite   nRoom=364   *(c->NWGameServer[i]->GameType) = 2;
// Melee        nRoom=365   *(c->NWGameServer[i]->GameType) = 5;
// Arena        nRoom=366   *(c->NWGameServer[i]->GameType) = 6;
// PW Story     nRoom=367   *(c->NWGameServer[i]->GameType) = 10;
// Solo         nRoom=368   *(c->NWGameServer[i]->GameType) = 11;
// Tech Support nRoom=370   *(c->NWGameServer[i]->GameType) = 12;
int RoomToSkywing(int room) {

    switch(room) {
    case 274:
        return 0; break;
    case 275:
        return 3; break;
    case 276:
        return 4; break;
    case 277:
        return 7; break;
    case 278:
        return 9; break;
    case 279:
        return 8; break;
    case 363:
        return 1; break;
    case 364:
        return 2; break;
    case 365:
        return 5; break;
    case 366:
        return 6; break;
    case 367:
        return 10; break;
    case 368:
        return 11; break;
    case 370:
        return 12; break;
    default:
        return 3; break;  // just put it in Roleplay :P
    }
}

const char * GetSoapErrorMessage(int res)
{

    if(soap_xml_error_check(res)) {
        return "Server Listing Failed.  It was an XML issue.\n";
    }
    else if(soap_soap_error_check(res)) {
        "Server Listing Failed.  SOAP itself had some kind of problem.\n";
    }
    else if(soap_tcp_error_check(res)) {
        "Server Listing Failed.  There was a TCP problem.\n";
    }
    else if(soap_ssl_error_check(res)) {
        "Server Listing Failed.  There was a SSL problem.\n";
    }
    else if(soap_zlib_error_check(res)) {
        "Server Listing Failed.  There was a ZLIB error!\n";
    }
    else if(soap_http_error_check(res)) {
        if(res == 415) {
            "Server Listing Failed.  Got back HTTP/Unsupported Media.  SOAP XML is probably for old bindings.\n";
        } else {
            "Server Listing Failed.  It was an HTTP error!\n";
        }
    }
    else {
        return "Server Listing Failed.  We have no idea why.  SOAP hates you.\n";

    }

    return "Undiagnosed error.\n";
}

unsigned int AddServer(const char *address) {
    if (!address || !*address) { return 0; }
    NWNMasterServerAPIProxy *api = new NWNMasterServerAPIProxy();
    api->soap_endpoint = API_ENDPOINT;

    auto servers = new char*[2];
    servers[0] = _strdup(address);
    servers[1] = NULL;

    RegisterPendingServers srv_request;
    RegisterPendingServersResponse srv_response;
    auto strings = new ns5__ArrayOfstring();
    strings->string = servers;
    strings->__sizestring = 1;

    srv_request.Product = (char *)soap_malloc(api, sizeof(char) * 5);  // NWN1 plus a null character.
    strcpy_s(srv_request.Product, sizeof(char) * 5,  "NWN1");
    srv_request.ServerAddresses = strings;

    int res = api->RegisterPendingServers(&srv_request, &srv_response);
    if(res != SOAP_OK) {
        delete api;
        return 0;
    }

    unsigned int r = *srv_response.RegisterPendingServersResult;
    delete api;
    return r;
}

