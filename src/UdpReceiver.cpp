#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using namespace std;
//
//
//
namespace net = boost::asio;
using udp =  net::ip::udp;

std::string make_daytime_string()
{
	auto now = chrono::system_clock::now();
	time_t now_time_t = chrono::system_clock::to_time_t(now);
	auto fraction = now - chrono::time_point_cast<chrono::seconds>(now);
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(fraction);

	std::tm now_tm = *std::localtime(&now_time_t);
	
	stringstream ss;
	ss << put_time(&now_tm, "%T") << "." << setw(3) << milliseconds.count();

	return ss.str();
}

class Receiver
{
public:
	Receiver(net::io_context& ioc, unsigned short port)
		: socket(ioc, udp::endpoint(udp::v4(), port))
	{
		cout << "Starting UDP Receiver on port [" << port << "] ..." << endl;
		start_receive();
	}

private:
	void start_receive()
	{
		socket.async_receive_from(
			net::buffer(recv_buffer)
		   ,remote_endpoint
		   ,boost::bind(&Receiver::handle_receive
					   ,this
					   ,net::placeholders::error
					   ,net::placeholders::bytes_transferred)
		);
	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transfered)
	{
		if (!error)
		{
			cout << "[" << make_daytime_string() << "] "
				<< "Received UDP data on port [" << socket.local_endpoint().port() << "]: '"
				<< string_view(recv_buffer.data(), bytes_transfered) << "'"
				<< endl;

			start_receive();
		}
	}
	
	udp::socket socket;
	udp::endpoint remote_endpoint;
	array<char, 256> recv_buffer;
};

void print_usage(const char* prog_name);
vector<unsigned short> read_ports(int argc, char** argv);

int main(int argc, char* argv[])
{
	auto ports = read_ports(argc, argv);

	if (ports.empty())
	{
		print_usage(argv[0]);
		return 1;
	}

	try {
		
		vector<Receiver> receivers;
		receivers.reserve(ports.size());

		cout << endl;

		net::io_context ioc;
		for (auto port : ports)
			receivers.emplace_back(ioc, port);

		cout << endl;

		ioc.run();
	
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void print_usage(const char* prog_name)
{
	printf("\nUsage: %s <port1> <port2> <port3> ... <portn>\n", prog_name);
}

vector<unsigned short> read_ports(int argc, char** argv)
{
	vector<unsigned short> ports;
	ports.reserve(argc - 1);
	for (int i = 1; i < argc; ++i) ports.push_back(static_cast<unsigned short>(stoul(argv[i])));
	return ports;
}
