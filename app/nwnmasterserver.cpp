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

