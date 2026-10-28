#include "socket_server.h"
#include <sys/stat.h>
#include <sys/types.h>

SocketServer::SocketServer(sc_module_name name) : sc_module(name) {
    server_fd = -1;
    client_fd = -1;
    socket_path = DTWIN_DEFAULT_SOCK_PATH;

    SC_THREAD(run_socket_server);
    sensitive << clk.pos();
}

SocketServer::~SocketServer() {
    close_client();
    close_server();
}

bool SocketServer::init_socket() {
    // Criar diretÃ³rio se nÃ£o existir
    std::string path_str(socket_path);
    std::string dir = path_str.substr(0, path_str.find_last_of('/'));
    if (!dir.empty()) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "mkdir -p %s", dir.c_str());
        system(cmd);
    }

    // Remover socket antigo
    unlink(socket_path);

    // Criar socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        SC_REPORT_ERROR("SOCKET", "socket() falhou");
        return false;
    }

    // Configurar permissÃµes (0666)
    umask(0000);

    // Bind
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        SC_REPORT_ERROR("SOCKET", "bind() falhou");
        close(server_fd);
        return false;
    }

    // PermissÃµes para nÃ£o-root
    chmod(socket_path, 0666);

    // Listen
    if (listen(server_fd, 5) < 0) {
        SC_REPORT_ERROR("SOCKET", "listen() falhou");
        close(server_fd);
        return false;
    }

    SC_REPORT_INFO("SOCKET", ("Socket criado em " + std::string(socket_path)).c_str());
    return true;
}

bool SocketServer::accept_client() {
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        SC_REPORT_ERROR("SOCKET", "accept() falhou");
        return false;
    }
    SC_REPORT_INFO("SOCKET", "QEMU conectado!");
    return true;
}

bool SocketServer::recv_packet(dtwin_req_packet_t& req) {
    ssize_t n = recv(client_fd, &req, sizeof(req), MSG_WAITALL);
    if (n != sizeof(req)) {
        SC_REPORT_WARNING("SOCKET", "recv() incompleto");
        return false;
    }
    if (req.magic != DTWIN_PROTO_MAGIC) {
        SC_REPORT_WARNING("SOCKET", "Magic bytes invalidos");
        return false;
    }
    return true;
}

bool SocketServer::send_packet(const dtwin_resp_packet_t& resp) {
    ssize_t n = send(client_fd, &resp, sizeof(resp), 0);
    if (n != sizeof(resp)) {
        SC_REPORT_WARNING("SOCKET", "send() incompleto");
        return false;
    }
    return true;
}

void SocketServer::close_client() {
    if (client_fd >= 0) {
        close(client_fd);
        client_fd = -1;
    }
}

void SocketServer::close_server() {
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }
}

void SocketServer::run_socket_server() {
    if (!init_socket()) {
        SC_REPORT_ERROR("SOCKET", "Falha ao inicializar socket");
        return;
    }

    while (true) {
        if (!accept_client()) {
            continue;
        }

        dtwin_req_packet_t req;
        while (recv_packet(req)) {
            std::cout << "[SOCKET] RX: cmd=0x" << std::hex << (int)req.cmd
                      << " addr=0x" << req.addr
                      << " data=0x" << req.data << std::dec << std::endl;

            // TODO: Encaminhar para Bridge (Etapa 4)
            dtwin_resp_packet_t resp;
            memset(&resp, 0, sizeof(resp));
            resp.magic = DTWIN_PROTO_MAGIC;
            resp.seq_id = req.seq_id;
            resp.status = DTWIN_STATUS_OK;
            resp.size = req.size;
            resp.data = req.data;

            send_packet(resp);
        }

        close_client();
        wait(10, SC_NS);
    }
}