#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <systemc.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <string>

#include "dtwin_ipc_protocol.h"

SC_MODULE(SocketServer) {
    sc_in<bool> clk;

    int server_fd;
    int client_fd;
    const char* socket_path;

    SC_CTOR(SocketServer);
    ~SocketServer();

    bool init_socket();
    bool accept_client();
    bool recv_packet(dtwin_req_packet_t& req);
    bool send_packet(const dtwin_resp_packet_t& resp);
    void close_client();
    void close_server();

    void run_socket_server();
};

#endif