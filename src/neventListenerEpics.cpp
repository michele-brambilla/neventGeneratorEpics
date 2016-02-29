#include <iostream>

#include "neventRequest.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;






static const double default_timeout = 3.0;
#define DEFAULT_REQUEST "field(value)"
#define DEFAULT_PROVIDER "pva"

double timeOut = default_timeout;
string request(DEFAULT_REQUEST);
string defaultProvider(DEFAULT_PROVIDER);
const string noAddress;

enum PrintMode { ValueOnlyMode, StructureMode, TerseMode };
PrintMode mode = ValueOnlyMode;




// char fieldSeparator = ' ';

// void usage (void)
// {
//   fprintf (stderr, "\nUsage: pvget [options] <PV name>...\n\n"
//            "  -h: Help: Print this message\n"
//            "options:\n"
//            "  -r <pv request>:   Request, specifies what fields to return and options, default is '%s'\n"
//            "  -w <sec>:          Wait time, specifies timeout, default is %f second(s)\n"
//            "  -t:                Terse mode - print only value, without names\n"
//            "  -i:                Do not format standard types (enum_t, time_t, ...)\n"
//            "  -m:                Monitor mode\n"
//            "  -p <provider>:     Set default provider name, default is '%s'\n"
//            "  -q:                Quiet mode, print only error messages\n"
//            "  -d:                Enable debug output\n"
//            "  -F <ofs>:          Use <ofs> as an alternate output field separator\n"
//            "  -f <input file>:   Use <input file> as an input that provides a list PV name(s) to be read, use '-' for stdin\n"
//            "  -c:                Wait for clean shutdown and report used instance count (for expert users)\n"
//            " enum format:\n"
//            "  -n: Force enum interpretation of values as numbers (default is enum string)\n"
//            //    " time format:\n"
//            //    "  -u: print userTag\n"
//            "\nexample: pvget double01\n\n"
//            , DEFAULT_REQUEST, DEFAULT_TIMEOUT, DEFAULT_PROVIDER);
// }

void printValue(std::string const & channelName, PVStructure::shared_pointer const & pv) {
  // if (mode == ValueOnlyMode)
  //   {
  PVField::shared_pointer value = pv->getSubField("value");
  if (value.get() == 0) {
    std::cerr << "no 'value' field" << std::endl;
    std::cout << channelName << std::endl << *(pv.get()) << std::endl << std::endl;
    // pvutil_ostream myos(std::cout.rdbuf());
    // myos << channelName << std::endl << *(pv.get()) << std::endl << std::endl;
  }
  else {
    Type valueType = value->getField()->getType();
    if (valueType != scalar && valueType != scalarArray) {
      // // switch to structure mode, unless it's T-type
      // if (valueType == structure && isTType(static_pointer_cast<PVStructure>(value))) {
      //   std::cout << std::setw(30) << std::left << channelName;
      //   //        std::cout << fieldSeparator;
      //   std::cout << " ";
      //   formatTType(std::cout, static_pointer_cast<PVStructure>(value));
      //   std::cout << std::endl;
      // }
      // else {
      std::cout << channelName << std::endl << *(pv.get()) << std::endl << std::endl;
        // pvutil_ostream myos(std::cout.rdbuf());
        // myos << channelName << std::endl << *(pv.get()) << std::endl << std::endl;
    // }
    }
    else {
      if (// fieldSeparator == ' ' &&
          value->getField()->getType() == scalar)
        std::cout << std::setw(30) << std::left << channelName;
      else
        std::cout << channelName;
      
      std::cout << " ";// fieldSeparator;
      
      //      terse(std::cout, value) << std::endl;
    }
  }
// }
//  else if (mode == TerseMode)
//    terseStructure(std::cout, pv) << std::endl;
//  else
//     {
//       //std::cout << channelName << std::endl << *(pv.get()) << std::endl << std::endl;
//       pvutil_ostream myos(std::cout.rdbuf());
//       myos << channelName << std::endl << *(pv.get()) << std::endl << std::endl;
//     }
}



// class MonitorRequesterImpl : public MonitorRequester
// {
// private:

//   string m_channelName;

// public:

//   MonitorRequesterImpl(std::string channelName) : m_channelName(channelName) {};

//   virtual string getRequesterName()
//   {
//     return "MonitorRequesterImpl";
//   };

//   virtual void message(std::string const & message,MessageType messageType)
//   {
//     std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
//   }

//   virtual void monitorConnect(const epics::pvData::Status& status, Monitor::shared_pointer const & monitor, StructureConstPtr const & /*structure*/)
//   {
//     if (status.isSuccess())
//       {
//         /*
//           string str;
//           structure->toString(&str);
//           std::cout << str << std::endl;
//         */

//         Status startStatus = monitor->start();
//         // show error
//         // TODO and exit
//         if (!startStatus.isSuccess())
//           {
//             std::cerr << "[" << m_channelName << "] channel monitor start: " << startStatus << std::endl;
//           }

//       }
//     else
//       {
//         std::cerr << "monitorConnect(" << status << ")" << std::endl;
//       }
//   }

//   virtual void monitorEvent(Monitor::shared_pointer const & monitor)
//   {

//     MonitorElement::shared_pointer element;
//     while ((element = monitor->poll()))
//       {
//         if (mode == ValueOnlyMode)
//           {
//             PVField::shared_pointer value = element->pvStructurePtr->getSubField("value");
//             if (value.get() == 0)
//               {
//                 std::cerr << "no 'value' field" << std::endl;
//                 std::cout << m_channelName << std::endl;
//                 //std::cout << *(element->pvStructurePtr.get()) << std::endl << std::endl;
//                 pvutil_ostream myos(std::cout.rdbuf());
//                 myos << *(element->pvStructurePtr.get()) << std::endl << std::endl;
//               }
//             else
//               {
//                 Type valueType = value->getField()->getType();
//                 if (valueType != scalar && valueType != scalarArray)
//                   {
//                     // switch to structure mode, unless it's T-type
//                     if (valueType == structure && isTType(static_pointer_cast<PVStructure>(value)))
//                       {
//                         std::cout << std::setw(30) << std::left << m_channelName;
//                         std::cout << fieldSeparator;
//                         formatTType(std::cout, static_pointer_cast<PVStructure>(value));
//                         std::cout << std::endl;
//                       }
//                     else
//                       {
//                         std::cout << m_channelName << std::endl;
//                         //std::cout << *(element->pvStructurePtr.get()) << std::endl << std::endl;
//                         pvutil_ostream myos(std::cout.rdbuf());
//                         myos << *(element->pvStructurePtr.get()) << std::endl << std::endl;
//                       }
//                   }
//                 else
//                   {
//                     if (fieldSeparator == ' ' && value->getField()->getType() == scalar)
//                       std::cout << std::setw(30) << std::left << m_channelName;
//                     else
//                       std::cout << m_channelName;

//                     std::cout << fieldSeparator;

//                     terse(std::cout, value) << std::endl;
//                   }
//               }
//           }
//         else if (mode == TerseMode)
//           {
//             if (fieldSeparator == ' ')
//               std::cout << std::setw(30) << std::left << m_channelName;
//             else
//               std::cout << m_channelName;

//             std::cout << fieldSeparator;

//             terseStructure(std::cout, element->pvStructurePtr) << std::endl;
//           }
//         else
//           {
//             std::cout << m_channelName << std::endl;
//             //std::cout << *(element->pvStructurePtr.get()) << std::endl << std::endl;
//             pvutil_ostream myos(std::cout.rdbuf());
//             myos << *(element->pvStructurePtr.get()) << std::endl << std::endl;
//           }

//         monitor->release(element);
//       }

//   }

//   virtual void unlisten(Monitor::shared_pointer const & /*monitor*/)
//   {
//     std::cerr << "unlisten" << std::endl;
//   }
// };



/*+**************************************************************************
 *
 * Function:	main
 *
 * Description:	pvget main()
 * 		Evaluate command line options, set up PVA, connect the
 * 		channels, print the data as requested
 *
 * Arg(s) In:	[options] <pv-name>...
 *
 * Arg(s) Out:	none
 *
 * Return(s):	Standard return code (0=success, 1=error)
 *
 **************************************************************************-*/

int main (int argc, char *argv[])
{
  // int opt;                    /* getopt() current option */
  // bool debug = false;
  // bool cleanupAndReport = false;
  // bool monitor = false;
  // bool quiet = false;

  // istream* inputStream = 0;
  // ifstream ifs;
  // bool fromStream = false;

  // setvbuf(stdout,NULL,_IOLBF,BUFSIZ);    /* Set stdout to line buffering */

  // while ((opt = getopt(argc, argv, ":hr:w:tmp:qdcF:f:ni")) != -1) {
  //   switch (opt) {
  //   case 'h':               /* Print usage */
  //     usage();
  //     return 0;
  //   case 'w':               /* Set PVA timeout value */
  //     if((epicsScanDouble(optarg, &timeOut)) != 1 || timeOut <= 0.0)
  //       {
  //         fprintf(stderr, "'%s' is not a valid timeout value "
  //                 "- ignored. ('pvget -h' for help.)\n", optarg);
  //         timeOut = DEFAULT_TIMEOUT;
  //       }
  //     break;
  //   case 'r':               /* Set PVA timeout value */
  //     request = optarg;
  //     // do not override terse mode
  //     if (mode == ValueOnlyMode) mode = StructureMode;
  //     break;
  //   case 't':               /* Terse mode */
  //     mode = TerseMode;
  //     break;
  //   case 'i':               /* T-types format mode */
  //     formatTTypes(false);
  //     break;
  //   case 'm':               /* Monitor mode */
  //     monitor = true;
  //     break;
  //   case 'p':               /* Set default provider */
  //     defaultProvider = optarg;
  //     break;
  //   case 'q':               /* Quiet mode */
  //     quiet = true;
  //     break;
  //   case 'd':               /* Debug log level */
  //     debug = true;
  //     break;
  //   case 'c':               /* Clean-up and report used instance count */
  //     cleanupAndReport = true;
  //     break;
  //   case 'F':               /* Store this for output formatting */
  //     fieldSeparator = (char) *optarg;
  //     break;
  //   case 'f':               /* Use input stream as input */
  //     {
  //       string fileName = optarg;
  //       if (fileName == "-")
  //         inputStream = &cin;
  //       else
  //         {
  //           ifs.open(fileName.c_str(), ifstream::in);
  //           if (!ifs)
  //             {
  //               fprintf(stderr,
  //                       "Failed to open file '%s'.\n",
  //                       fileName.c_str());
  //               return 1;
  //             }
  //           else
  //             inputStream = &ifs;
  //         }

  //       fromStream = true;
  //       break;
  //     }
  //   case 'n':
  //     setEnumPrintMode(NumberEnum);
  //     break;
  //   case '?':
  //     fprintf(stderr,
  //             "Unrecognized option: '-%c'. ('pvget -h' for help.)\n",
  //             optopt);
  //     return 1;
  //   case ':':
  //     fprintf(stderr,
  //             "Option '-%c' requires an argument. ('pvget -h' for help.)\n",
  //             optopt);
  //     return 1;
  //   default :
  //     usage();
  //     return 1;
  //   }
  // }

  int nPvs = argc - optind;       /* Remaining arg list are PV names */
  // if (nPvs > 0)
  //   {
  //     // do not allow reading file and command line specified pvs
  //     fromStream = false;
  //   }
  // else if (nPvs < 1 && !fromStream)
  //   {
  //     fprintf(stderr, "No pv name(s) specified. ('pvget -h' for help.)\n");
  //     return 1;
  //   }

  vector<string> pvs;     /* Array of PV structures */
  // if (fromStream)
  //   {
  //     string cn;
  //     while (true)
  //       {
  //         *inputStream >> cn;
  //         if (!(*inputStream))
  //           break;
  //         pvs.push_back(cn);
  //       }

  //     // set nPvs
  //     nPvs = pvs.size();
  //   }
  // else
  //   {
  // copy PV names from command line
  for (int n = 0; optind < argc; n++, optind++) {
    pvs.push_back(argv[optind]);
  }

  // SET_LOG_LEVEL(debug ? logLevelDebug : logLevelError);

  std::cout << std::boolalpha;
  // terseSeparator(fieldSeparator);

  bool allOK = true;
  
  {
    Requester::shared_pointer requester(new RequesterImpl("neventListenerEpics"));
    
    PVStructure::shared_pointer pvRequest = CreateRequest::create()->createRequest(request);
    if(pvRequest.get()==NULL) {
      fprintf(stderr, "failed to parse request string\n");
      return 1;
    }
    
    std::vector<std::string> pvNames;
    std::vector<std::string> pvAddresses;
    std::vector<std::string> providerNames;
    
    pvNames.reserve(nPvs);
    pvAddresses.reserve(nPvs);
    providerNames.reserve(nPvs);
    
    for (int n = 0; n < nPvs; n++) {
      URI uri;
      bool validURI = URI::parse(pvs[n], uri);
      
      std::string providerName(defaultProvider);
      std::string pvName(pvs[n]);
      std::string address(noAddress);
      bool usingDefaultProvider = true;
      if (validURI) {
        if (uri.path.length() <= 1) {
          std::cerr << "invalid URI '" << pvs[n] << "', empty path" << std::endl;
          return 1;
        }
        providerName = uri.protocol;
        pvName = uri.path.substr(1);
        address = uri.host;
        usingDefaultProvider = false;
      }
      
      if ((providerName != "pva") && (providerName != "ca")) {
        std::cerr << "invalid "
                  << (usingDefaultProvider ? "default provider" : "URI scheme")
                  << " '" << providerName 
                  << "', only 'pva' and 'ca' are supported" << std::endl;
        return -1;
      }
      pvNames.push_back(pvName);
      pvAddresses.push_back(address);
      providerNames.push_back(providerName);

      std:: cout << "pvNames "       << pvNames[n]       << "\n"
                 << "pvAddresses "   << pvAddresses[n]   << "\n"
                 << "providerNames " << providerNames[n] << "\n"
                 << std::endl;
    }
    
    // start both the pva and ca client providers
    ClientFactory::start();
    epics::pvAccess::ca::CAClientFactory::start();
    
    // first connect to all, this allows resource (e.g. TCP connection) sharing
    vector<Channel::shared_pointer> channels(nPvs);
    for (int n = 0; n < nPvs; n++) {
      shared_ptr<ChannelRequesterImpl> channelRequesterImpl(new ChannelRequesterImpl(true));
      if (pvAddresses[n].empty())
        channels[n] = getChannelProviderRegistry()->getProvider(
                                                                providerNames[n])->createChannel(pvNames[n],
                                                                                                 channelRequesterImpl);
      else
        channels[n] = getChannelProviderRegistry()->getProvider(
                                                                providerNames[n])->createChannel(pvNames[n],
                                                                                                 channelRequesterImpl,
                                                                                                 ChannelProvider::PRIORITY_DEFAULT,
                                                                                                 pvAddresses[n]);
    }

      // for now a simple iterating sync implementation, guarantees order
    for (int n = 0; n < nPvs; n++) {
      Channel::shared_pointer channel = channels[n];
      shared_ptr<ChannelRequesterImpl> channelRequesterImpl = dynamic_pointer_cast<ChannelRequesterImpl>(channel->getChannelRequester());
      

        
      if (channelRequesterImpl->waitUntilConnected(timeOut)) {
        shared_ptr<GetFieldRequesterImpl> getFieldRequesterImpl;
        
        // probe for value field
        // if (mode == ValueOnlyMode) {
        getFieldRequesterImpl.reset(new GetFieldRequesterImpl(channel));
        // get all to be immune to bad clients not supporting selective getField request
        channel->getField(getFieldRequesterImpl, "");
        
        
        if (getFieldRequesterImpl.get() == 0 ||
            getFieldRequesterImpl->waitUntilFieldGet(timeOut)) {
          // check probe
          if (getFieldRequesterImpl.get()) {
            Structure::const_shared_pointer structure =
              dynamic_pointer_cast<const Structure>(getFieldRequesterImpl->getField());
            if (structure.get() == 0 || structure->getField("value").get() == 0) {
              // fallback to structure
              mode = StructureMode;
              pvRequest = CreateRequest::create()->createRequest("field()");
            }
          }
        }
        
        int count = 0;
        while(true) {
          count++;
          
          
        //               if (!monitor)
        //                 {
          shared_ptr<ChannelGetRequesterImpl> getRequesterImpl(new ChannelGetRequesterImpl(channel->getChannelName()));
          ChannelGet::shared_pointer channelGet = channel->createChannelGet(getRequesterImpl, pvRequest);
          allOK &= getRequesterImpl->waitUntilGet(timeOut);
          
          if(count %140 ==0) {
            if (allOK)
              printValue(channel->getChannelName(), getRequesterImpl->getPVStructure());
            else {
              allOK = false;
              channel->destroy();
              std::cerr << "[" << channel->getChannelName() << "] failed to get channel introspection data" << std::endl;
            }
            //         }
        //       else
        //         {
        //           allOK = false;
        //           channel->destroy();
        //           std::cerr << "[" << channel->getChannelName() << "] connection timeout" << std::endl;
        //         }
          }    
        }
        //   if (allOK && monitor)
        //     {
        //       while (true)
        //         epicsThreadSleep(timeOut);
        //     }
      }
    }   
    //   epics::pvAccess::ca::CAClientFactory::stop();
      //   ClientFactory::stop();
    
    }
  
    // if (cleanupAndReport){
    //   // TODO implement wait on context
    //   epicsThreadSleep ( 3.0 );
    //   //std::cout << "-----------------------------------------------------------------------" << std::endl;
    //   //epicsExitCallAtExits();
    // }
    
    return allOK ? 0 : 1;
    return 0;
  }
