#include "server.hpp"

#include <algorithm>
#include <functional>
#include <iostream>

WebsocketServer::WebsocketServer()
{
    //Wire up our event handlers
    this->endpoint.set_open_handler(std::bind(&WebsocketServer::onOpen, this, std::placeholders::_1));
    this->endpoint.set_close_handler(std::bind(&WebsocketServer::onClose, this, std::placeholders::_1));
    this->endpoint.set_message_handler(std::bind(&WebsocketServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    
    //Initialise the Asio library, using our own event loop object
    this->endpoint.init_asio(&(this->eventLoop));
}

void WebsocketServer::run(int port)
{
    //Listen on the specified port number and start accepting connections
    this->endpoint.listen(port);
    this->endpoint.start_accept();
    
    //Start the Asio event loop
    this->endpoint.run();
}

size_t WebsocketServer::numConnections()
{
    //Prevent concurrent access to the list of open connections from multiple threads
    std::lock_guard<std::mutex> lock(this->connectionListMutex);
    
    return this->openConnections.size();
}

void WebsocketServer::sendMessage(ClientConnection conn, const string& message)
{
    //Send the JSON data to the client (will happen on the networking thread's event loop)
    this->endpoint.send(conn, message, websocketpp::frame::opcode::text);
}

void WebsocketServer::broadcastMessage(const string& message)
{
    //Prevent concurrent access to the list of open connections from multiple threads
    std::lock_guard<std::mutex> lock(this->connectionListMutex);
    
    for (auto conn : this->openConnections) {
        this->sendMessage(conn, message);
    }
}

void WebsocketServer::onOpen(ClientConnection conn)
{
    {
        //Prevent concurrent access to the list of open connections from multiple threads
        std::lock_guard<std::mutex> lock(this->connectionListMutex);
        
        //Add the connection handle to our list of open connections
        this->openConnections.push_back(conn);
    }
    
    //Invoke any registered handlers
    for (auto handler : this->connectHandlers) {
        handler(conn);
    }
}

void WebsocketServer::onClose(ClientConnection conn)
{
    {
        //Prevent concurrent access to the list of open connections from multiple threads
        std::lock_guard<std::mutex> lock(this->connectionListMutex);
        
        //Remove the connection handle from our list of open connections
        auto connVal = conn.lock();
        auto newEnd = std::remove_if(this->openConnections.begin(), this->openConnections.end(), [&connVal](ClientConnection elem)
        {
            //If the pointer has expired, remove it from the vector
            if (elem.expired() == true) {
                return true;
            }
            
            //If the pointer is still valid, compare it to the handle for the closed connection
            auto elemVal = elem.lock();
            if (elemVal.get() == connVal.get()) {
                return true;
            }
            
            return false;
        });
        
        //Truncate the connections vector to erase the removed elements
        this->openConnections.resize(std::distance(openConnections.begin(), newEnd));
    }

    //Invoke any registered handlers
    for (auto handler : this->disconnectHandlers) {
        handler(conn);
    }
}

void WebsocketServer::onMessage(ClientConnection conn, WebsocketEndpoint::message_ptr msg)
{
    //Validate that the incoming message contains valid JSON
    string message = msg->get_payload();

    for (auto handler : this->messageHandlers) {
        handler(conn, message);
    }
}
