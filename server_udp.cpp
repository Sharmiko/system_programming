#include <array>
#include <iostream>
#include <stdexcept>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 22000
#define MAX_SIZE 0x10


class Server {
private:
    int m_fd {};
    struct sockaddr_in m_addr {};
    struct sockaddr_in m_client {};
public:
    explicit Server(uint16_t port)
    {
        if (this->m_fd = ::socket(AF_INET, SOCK_DGRAM, 0); this->m_fd == -1)
        {
            throw std::runtime_error(strerror(errno));
        }

        this->m_addr.sin_family = AF_INET;
        this->m_addr.sin_port = ::htons(port);
        this->m_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

        if (this->bind() == -1)
        {
            throw std::runtime_error(strerror(errno));
        }
    }

    ~Server()
    {
        ::close(this->m_fd);
    }

    int bind()
    {
        return ::bind(
            this->m_fd,
            reinterpret_cast<struct sockaddr *>(&this->m_addr),
            sizeof(this->m_addr)
        );
    }

    ssize_t recv(std::array<char, MAX_SIZE> &buf)
    {
        socklen_t client_len = sizeof(this->m_client);

        return ::recvfrom(
            this->m_fd,
            buf.data(),
            buf.size(),
            0,
            (struct sockaddr *) &this->m_client,
            &client_len
        );
    }

    ssize_t send(std::array<char, MAX_SIZE> &buf, ssize_t len)
    {
        if (len >= buf.size())
        {
            throw std::out_of_range("len >= buf.size()");
        }

        return ::sendto(
            this->m_fd,
            buf.data(),
            buf.size(),
            0,
            (struct sockaddr *) &this->m_client,
            sizeof(this->m_client)
        );
    }

    void echo()
    {
        while(true)
        {
            std::array<char, MAX_SIZE> buf {};

            if (auto len = recv(buf); len != 0)
            {
                this->send(buf, len);
            }
            else
            {
                break;
            }
        }
    }
};



int protected_main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    Server server {PORT};
    server.echo();

    return EXIT_SUCCESS;
}


int main(int argc, char** argv)
{
    try 
    {
        return protected_main(argc, argv);
    }
    catch(const std::exception &e)
    {
        std::cerr << "Caught unhandled exception:\n";
        std::cerr << " - what(): " << e.what() << '\n';
    }
    catch(...)
    {
        std::cerr << "Caught unknown exception\n";
    }

    return EXIT_FAILURE;
}