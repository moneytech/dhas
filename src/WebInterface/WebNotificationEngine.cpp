#include "WebNotificationEngine.h"
#include "DHASLogging.h"
#include <sstream>

// This is used for Server Sent Events

WebNotificationEngine::WebNotificationEngine()
{
    pthread_mutex_init(&mSocketListLock,0);
}

WebNotificationEngine::~WebNotificationEngine()
{
    pthread_mutex_destroy(&mSocketListLock);
}

void WebNotificationEngine::sendData(int socket, const std::string& data)
{
    std::stringstream payload;
    payload << std::hex << (data.size()+2) << "\r\n" << data << "\n\n\r\n";

    send(socket,payload.str().c_str(),payload.str().size(),0); 
}

void WebNotificationEngine::activateSubscription(int socket)
{
    pthread_mutex_lock(&mSocketListLock);
    if (mSocketList.find(socket) != mSocketList.end())
    {
        mSocketList[socket] = true;
    }
    pthread_mutex_unlock(&mSocketListLock);

    sendData(socket,"retry: 1000");
}

void WebNotificationEngine::subscribe(int socket)
{
    LOG("A client is subscribing to notifications "<<socket);
    pthread_mutex_lock(&mSocketListLock);
    mSocketList[socket]= false;
    pthread_mutex_unlock(&mSocketListLock);
}

void WebNotificationEngine::unsubscribe(int socket)
{
    pthread_mutex_lock(&mSocketListLock);
    if (mSocketList.find(socket) != mSocketList.end())
    {
        mSocketList.erase(socket);
        LOG("A client is unsubscribing to notifications "<<socket);
    }
    pthread_mutex_unlock(&mSocketListLock);
}

void WebNotificationEngine::notifyEvent(const Dumais::JSON::JSON& json)
{
    std::string jsonEvent = json.stringify(false);
    std::string st = "data: ";
    st += jsonEvent;

    for (std::map<int,bool>::iterator it = mSocketList.begin(); it!=mSocketList.end();it++)
    {
        if (it->second == true)
        {
            sendData(it->first,st);
        }
    }
}
