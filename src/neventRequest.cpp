#include "neventRequest.h"

using namespace epics::pvData;
using namespace epics::pvAccess;

using namespace std;



RequesterImpl::RequesterImpl(std::string const & requesterName) : requesterName(requesterName) { }

std::string RequesterImpl::getRequesterName() { return requesterName; }

void RequesterImpl::message(std::string const & message, MessageType message_t) {
  std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(message_t) << ")" << std::endl;
}




/**********************************************/




ChannelRequesterImpl::ChannelRequesterImpl(bool _printOnlyErrors) :
    printOnlyErrors(_printOnlyErrors), showDisconnectMsg(false)
{
}

string ChannelRequesterImpl::getRequesterName()
{
	return "ChannelRequesterImpl";
}

void ChannelRequesterImpl::message(std::string const & message, MessageType messageType)
{
    if (!printOnlyErrors || messageType > warningMessage)
        std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
}

void ChannelRequesterImpl::channelCreated(const epics::pvData::Status& status, Channel::shared_pointer const & channel)
{
	if (status.isSuccess())
	{
		// show warning
		if (!status.isOK())
		{
            std::cerr << "[" << channel->getChannelName() << "] channel create: " << // dump_stack_only_on_debug(status) <<
              std::endl;
		}
	}
	else
	{
        std::cerr << "[" << channel->getChannelName() << "] failed to create a channel: " // << dump_stack_only_on_debug(status)
                  << std::endl;
	}
}

void ChannelRequesterImpl::channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState)
{
	if (connectionState == Channel::CONNECTED)
	{
		m_event.signal();
	}
    else if (showDisconnectMsg && connectionState == Channel::DISCONNECTED)
    {
        std::cerr << std::setw(30) << std::left << channel->getChannelName()
                  << ' ' << "*** disconnected" << std::endl;
    }
    /*
	else if (connectionState != Channel::DESTROYED)
	{
		std::cerr << "[" << channel->getChannelName() << "] channel state change: "  << Channel::ConnectionStateNames[connectionState] << std::endl;
	}
	*/
}
    
bool ChannelRequesterImpl::waitUntilConnected(double timeOut)
{
	return m_event.wait(timeOut);
}

void ChannelRequesterImpl::showDisconnectMessage(bool show)
{
    showDisconnectMsg = show;
}

/********************************/

// ChannelRequesterImpl::ChannelRequesterImpl(bool _printOnlyErrors) :
//     printOnlyErrors(_printOnlyErrors), showDisconnectMsg(false)
// {
// }

// string ChannelRequesterImpl::getRequesterName()
// {
// 	return "ChannelRequesterImpl";
// }

// void ChannelRequesterImpl::message(std::string const & message, MessageType messageType)
// {
//     if (!printOnlyErrors || messageType > warningMessage)
//         std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
// }

// void ChannelRequesterImpl::channelCreated(const epics::pvData::Status& status, Channel::shared_pointer const & channel)
// {
// 	if (status.isSuccess())
// 	{
// 		// show warning
// 		if (!status.isOK())
// 		{
//             std::cerr << "[" << channel->getChannelName() << "] channel create: " << dump_stack_only_on_debug(status) << std::endl;
// 		}
// 	}
// 	else
// 	{
//         std::cerr << "[" << channel->getChannelName() << "] failed to create a channel: " << dump_stack_only_on_debug(status) << std::endl;
// 	}
// }

// void ChannelRequesterImpl::channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState)
// {
// 	if (connectionState == Channel::CONNECTED)
// 	{
// 		m_event.signal();
// 	}
//     else if (showDisconnectMsg && connectionState == Channel::DISCONNECTED)
//     {
//         std::cerr << std::setw(30) << std::left << channel->getChannelName()
//                   << ' ' << "*** disconnected" << std::endl;
//     }
//     /*
// 	else if (connectionState != Channel::DESTROYED)
// 	{
// 		std::cerr << "[" << channel->getChannelName() << "] channel state change: "  << Channel::ConnectionStateNames[connectionState] << std::endl;
// 	}
// 	*/
// }
    
// bool ChannelRequesterImpl::waitUntilConnected(double timeOut)
// {
// 	return m_event.wait(timeOut);
// }

// void ChannelRequesterImpl::showDisconnectMessage(bool show)
// {
//     showDisconnectMsg = show;
// }



