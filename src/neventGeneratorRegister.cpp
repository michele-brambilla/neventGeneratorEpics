#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/pvDatabase.h>
#include <neventDataRecord.h>

#include <epicsExport.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::nevent;

static const iocshArg testArg0 = { "record_name", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef neventDatabaseFuncDef = {
    "neventDatabase", 1, testArgs};
static void neventDatabaseCallFunc(const iocshArgBuf *args)
{
    neventDataRecord::create(args[0].sval);
}

static void neventDatabaseRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&neventDatabaseFuncDef, neventDatabaseCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(neventDatabaseRegister);
}
