#include <array>
#include <string>
#include <iostream>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 22000
#define MAX_SIZE 0x10


class Client {
private:
    int m_fd {};
    struct sockaddr_in m_addr {};
public:
    explicit Client(uint16_t port)
    {
        if (this->m_fd = ::socket(AF_INET, SOCK_STREAM, 0); this->m_fd == -1)
        {
            throw std::runtime_error(strerror(errno));
        }

        this->m_addr.sin_family = AF_INET;
        this->m_addr.sin_port = ::htons(port);
        this->m_addr.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);

        if (this->connect() == 1)
        {
            throw std::runtime_error(strerror(errno));
        }
    }

    ~Client()
    {
        ::close(this->m_fd);
    }

    int connect()
    {
        return ::connect(
            this->m_fd,
            reinterpret_cast<struct sockaddr *>(&this->m_addr),
            sizeof(this->m_addr)
        );
    }

    ssize_t send(const std::string &buf)
    {
        return ::send(
            this->m_fd,
            buf.data(),
            buf.size(),
            0
        );
    }

    ssize_t recv(std::array<char, MAX_SIZE> &buf)
    {
        return ::recv(
            this->m_fd,
            buf.data(),
            buf.size() - 1,
            0
        );
    }


    void echo()
    {
        while (true)
        {
            std::string sendbuf {};
            std::array<char, MAX_SIZE> recvbuf {};

            std::cin >> sendbuf;

            this->send(sendbuf);
            this->recv(recvbuf);

            std::cout << recvbuf.data() << '\n';
        }
    }
};


int protected_main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    Client client {PORT};
    client.echo();

    return EXIT_SUCCESS;
}


int main(int argc, char** argv)
{
    try
    {
        return protected_main(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught unhandled exception:\n";
        std::cerr << " - what(): " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Caught unknown exception\n";
    }

    return EXIT_FAILURE;
}