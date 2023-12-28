#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include <csignal>
#include <thread>
#include <memory>
#include <iostream>


//#include "netsender.h"
#include "protocol_interface.h"

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
	}

	virtual ~protocol_echo() {
	    printf("protocol_echo destructor\n");
	}

};

int main(int argc, char* argv[])
{
    signal( SIGINT, sig_handler);

    shared_ptr<protocol_interface> protocol;
    protocol.reset(new protocol_echo());
    protocol->create_sender(netsender::PROTOCOL_TCP, netsender::TYPE_CLIENT, "127.0.0.1", 8003);
//    shared_ptr<netsender> pSender;
//    pSender.reset(netsender::createSender(netsender::PROTOCOL_TCP, netsender::TYPE_CLIENT, "127.0.0.1", 8003, protocol.get()));

//    if(pSender == nullptr)
//    {
//	cout << " createSender error!" << endl;
//	return -1;
//    }

//    bool bRet;
//    bRet = protocol->isConnect();
//    cout << "isconnect = " << bRet << endl;

    int count = 0;
    char buf[512];
//    while(keepRunning)
    for (int i = 0; i < 3; ++i)
    {
	sprintf(buf, "send NO %d \n", count);  //回复client
	cout << buf << endl;
//	pSender->send_buf(buf);
	protocol->send_data(buf);
	++count;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    printf("program exit!\n");

    return 0;
}
