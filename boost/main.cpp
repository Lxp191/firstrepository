#include <iostream>
#include <memory>
#include "Boost/asio.hpp"
#include <vld.h>
using namespace std;

int acceptNum = 0;

class Session : public enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket) : m_socket(std::move(socket)) {}

    void async_read()
    {
        m_socket.async_read_some(boost::asio::buffer(m_buffer), [this, self = shared_from_this()](boost::system::error_code ec, std::size_t length)
                                 {
            if (!ec)
            {
                std::cout << "Read " << length << " bytes" << std::endl;
                cout<<string(m_buffer.begin(),m_buffer.begin()+length)<<endl;
            }
            else if(ec ==boost::asio::error::eof||ec==boost::asio::error::operation_aborted||ec==boost::asio::error::connection_reset){
                cout<<"socket close  "<< ec <<endl;
                return;
            }
        
            async_write(length); });
    }

    void async_write(size_t writeSize)
    {
        boost::asio::async_write(m_socket, boost::asio::buffer(m_buffer, writeSize), [this, self = shared_from_this()](boost::system::error_code ec, std::size_t length)
                                 {
            if (!ec)
            {
                std::cout << "Write " << length << " bytes" << std::endl;
            } 
            else if(ec ==boost::asio::error::eof||ec==boost::asio::error::operation_aborted||ec==boost::asio::error::connection_reset){
                cout<<"socket close  "<< ec <<endl;
                return;
            }
        
            async_read(); });
    }

private:
    boost::asio::ip::tcp::socket m_socket;
    array<char, 1024> m_buffer;
};

class Server
{
public:
    Server(boost::asio::io_context *io_context, boost::asio::ip::tcp::acceptor *acceptor)
        : m_io_context(io_context), m_acceptor(acceptor) {}

    ~Server()
    {
    }

    void run()
    {
        wait_for_connection();
        cout << "run" << endl;
        m_io_context->run();
    }

private:
    void wait_for_connection()
    {
        m_acceptor->async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
                                 {
            if (!ec)
            {
                std::cout << "Connection accepted" << std::endl;

                auto session = make_shared<Session>(std::move(socket));
                session->async_read();
            }

            acceptNum++;
            if(acceptNum>5)
            {
                cout<<"stop"<<endl;
                m_io_context->stop();
                return;
            }

            wait_for_connection(); });
    }

private:
    boost::asio::io_context *m_io_context;
    boost::asio::ip::tcp::acceptor *m_acceptor;
};

int main()
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));
    Server s(&io_context, &acceptor);
    s.run();
    return 0;
}