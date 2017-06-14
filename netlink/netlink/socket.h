/*
    NetLink Sockets: Networking C++ library
    Copyright 2012 Pedro Francisco Pareja Ruiz (PedroPareja@Gmail.com)

    This file is part of NetLink Sockets.

    NetLink Sockets is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NetLink Sockets is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NetLink Sockets. If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __NL_SOCKET
#define __NL_SOCKET

#include "netlink/core.h"


NL_NAMESPACE

/**
* @class Socket socket.h netlink/socket.h
*
* Socket class
*
* @note The Exceptions with asterisk(*) includes the native error code (Exception::nativeErrorCode()) when thrown
* @warning Remember to call init() before using Socket
*/

class Socket {

    private:

        string      _hostTo;
        string      _hostFrom;
        unsigned    _portTo;
        unsigned    _portFrom;
        Protocol    _protocol;
        IPVer       _ipVer;
        SocketType  _type;
        bool        _blocking;
        unsigned    _listenQueue;

        int         _socketHandler;


    public:

        Socket(const string& hostTo, unsigned portTo, Protocol protocol = TCP, IPVer ipVer = ANY);

        Socket(unsigned portFrom, Protocol protocol = TCP, IPVer ipVer = IP4, const string& hostFrom = "", unsigned listenQueue = DEFAULT_LISTEN_QUEUE);

        Socket(const string& hostTo, unsigned portTo, unsigned portFrom, IPVer ipVer = ANY);

        ~Socket();


        Socket* accept();

        int read(void* buffer, size_t bufferSize);
        void send(const void* buffer, size_t size);
    
        //zhangqi, 2015-07-17, 新加可以给socket send函数指定flag参数的成员方法，方便iOS和Android兼容避免SIGPIPE信号中断导致崩溃的处理
        void sendWithFlag(const void* buffer, size_t size, int flag = 0);

        int readFrom(void* buffer, size_t bufferSize, string* HostFrom, unsigned* portFrom = NULL);
        void sendTo(const void* buffer, size_t size, const string& hostTo, unsigned portTo);

        int nextReadSize() const;

        void disconnect();

        const string&   hostTo() const;
        const string&   hostFrom() const;
        unsigned        portTo() const;
        unsigned        portFrom() const;
        Protocol        protocol() const;
        IPVer           ipVer() const;
        SocketType      type() const;
        bool            blocking() const;
        unsigned        listenQueue() const;
        int             socketHandler() const;


        void blocking(bool blocking);


    private:
        int connect_noblock(struct addrinfo *res); //zhangqi, 2015-08-20
        void initSocket();
        Socket();

};

#include <netlink/socket.inline.h>

NL_NAMESPACE_END

#endif
