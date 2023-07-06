
#include <iostream>
#include <csignal>

#include "protocol_interface.h"
#include "netsender.h"

using namespace std;

int keepRunning = 1;

void sig_handler( int sig )
{
    if ( sig == SIGINT)
    {
	keepRunning = 0;
    }
}

int main()
{
    shared_ptr<protocol_interface> protocol;
    protocol.reset(new protocol_interface());

    shared_ptr<netsender> pSender;
    pSender.reset(netsender::createSender(netsender::PROTOCOL_UDP, netsender::TYPE_SERVER, "", 8003, protocol.get()));

    while(keepRunning)
    {
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

