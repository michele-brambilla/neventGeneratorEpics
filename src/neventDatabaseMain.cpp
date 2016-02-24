#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <pv/serverContext.h>

#include <epicsExport.h>

#include <pv/channelProviderLocal.h>
#include <neventDataRecord.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::nevent;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    
    std::cout << "Creating PV(s)" << std::endl;
    
    neventDataRecord::shared_pointer ned = neventDataRecord::create("nevent1");
    master->addRecord(ned);

    std::cout << "Done creating PV(s)" << std::endl;
    
    ServerContext::shared_pointer ctx =
        startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    PVStringArrayPtr pvNames = master->getRecordNames();
    cout << "Record Names:" << endl << *pvNames << endl;
    string str;
    
    clock_t time_end;
    time_end = clock();
    
    while(true) {
	// A crude way to make the data update at ~14 Hz
	// At least on my machine!
        if (clock() >= time_end)
        {
	    time_end = clock() + (71.429/2.0) * CLOCKS_PER_SEC/1000.0;
	    ned->update(argv[1]);         
        }
        usleep(500);
    }
    ctx->destroy();
    epicsThreadSleep(1.0);
    channelProvider->destroy();
    return 0; 
}
