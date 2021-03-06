#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include "webserver/WebServer.h"
#include "RESTInterface.h"
#include "WebNotificationEngine.h"
#include <string>
#include <thread>

class WebInterface: public Dumais::WebServer::IWebServerListener
{
private:
    int mPort;
    RESTInterface *mpRESTInterface;
    Dumais::WebServer::WebServer* mpWebServer;
    WebNotificationEngine *mpWebNotificationEngine;
    int connectionCount;
    std::string mPasswdFile;
    std::thread *mpThread;
    std::string mSSLCert;
    std::string mSSLKey;
    bool        mStopped;

protected:
public:
	WebInterface(int port, RESTInterface *pRESTInterface, WebNotificationEngine *pWebNotificationEngine);
	~WebInterface();

    void start();
    void stop();
    void configure(Dumais::JSON::JSON& config);

    virtual Dumais::WebServer::HTTPResponse* processHTTPRequest(Dumais::WebServer::HTTPRequest* request);
    virtual void processHTTPRequestAsync(Dumais::WebServer::HTTPRequest*, Dumais::WebServer::HTTPRequestCallBack);
    virtual void onConnectionOpen();
    virtual void onConnectionClosed();
    virtual void onResponseSent();
};

#endif

