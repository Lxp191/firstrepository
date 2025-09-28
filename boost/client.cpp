#include <iostream>
#include "boost/asio.hpp"
using namespace std;

int main()
{
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 8080);
        socket.connect(endpoint);

        string message = "Hello, server!";
        boost::asio::write(socket, boost::asio::buffer(message));

        char data[1024];
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(data), error);
        cout << "Received: " << string(data, length) << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}