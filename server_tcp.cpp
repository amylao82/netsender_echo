
#include <iostream>
#include <csignal>

#include "protocol_interface.h"
//#include "netsender.h"

using namespace std;

int keepRunning = 1;

void sig_handler( int sig )
{
    if ( sig == SIGINT)
    {
	keepRunning = 0;
    }
}

class protocol_echo : public protocol_interface
{
    public:
	virtual void recv_data(shared_ptr<recv_packet> packet)
	{
	    string str(packet->vec.begin(), packet->vec.end());
	    printf("my protocol receive data len = %d, data = %s\n", packet->vec.size(), str.c_str());

	    send_data(str, packet->socketinfo);
	}

	virtual ~protocol_echo() {
	    printf("protocol_echo destructor\n");
	}

};

int main()
{
    signal( SIGINT, sig_handler);

    shared_ptr<protocol_interface> protocol;
    //protocol.reset(new protocol_interface());
    protocol = make_shared<protocol_echo>();

    protocol->create_sender(netsender::PROTOCOL_TCP, netsender::TYPE_SERVER, "", 8003);

    while(keepRunning)
    {
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

