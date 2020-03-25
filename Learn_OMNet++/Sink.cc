# include <iostream>
# include <omnetpp.h>
# include <queue>

using namespace omnetpp;
using namespace std;

class Sink :public cSimpleModule
{
    public:
        int X;
        int Y;
        int noDataInSink;
        double sleepTime;
        cQueue *sinkDateQueue;
        Sink();
        virtual ~Sink();
    
    private:
        void SetCoordinate();
        custMsg* CreateCustMsg(const char *name);
    
    private:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
}
Define_Module(Sink)

Sink::Sink()
{
    sleepTime= 0.5;
    noDataInSink=0;
}

Sink::~Sink()
{

}

void Sink::initialize()
{
    sinkDataQueue =new cQueue;
    custMsg *wakeUp =CreateCustMsg("wakeup");
    x = getParentModule()->par("sinkX");
    y = getParentModule()->par("sinkY");

    EV<<"SinkX :"<<X<<endl;
    EV<<"SinkY :"<<Y<<endl;

    SetCoordinate();
    scheduleAt(simTime().dbl()+0.5,wakeUp);
}

custMsg *Sink::CreateCustMsg(const char *name)
{
    custMsg *createMsg  =new custMsg(name);
    CreateMsg ->setPacketGenerateTime(simTime().dbl());
    createMsg->setPacketReachTime(simTime().dbl());
    createMsg->setSourceId(getIndex());
    return createMsg;
}

void Sink::handleMessage(cMessage *msg)
{
    custMsg *inMsg=check_and_cast<custMsg *>(msg);
    if(strcmp("DateMsg",inMsg->getFullname())==0)
    {
        noDataInSink++;
        inMsg->setPacketReachTime(simTime().dbl());
        sinkDateQueue->insert(inMsg);
    }
    if(msg->isSelfMessage())
    {
        scheduleAt(simTime().dbl()+sleepTime,msg->dup());
    }

}


void Sink::SetCoordinate()
{
    getDisplayString().setTagArg("p",0,X);
    getDisplayString().setTagArg("p",1,Y);
    getDisplayString().setTagArg("i",0,"device/antennatower");
}