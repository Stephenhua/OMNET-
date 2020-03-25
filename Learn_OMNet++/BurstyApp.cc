# include <omnetpp>
# include <iostream>
# include <vector>
using namespace std;
using namespace omnetpp;

class BurstyApp :public cSimpleModule
{
    private:
    int myAddress;
    vector<int> destAddresses;
    cPar *sleepTime;
    cPar *BurstyTime;
    cPar *sendIATime;
    cPar *packetLengthBytes;
    
    cFSM fsm;

    enum{
        INIT=0;
        SLEEP=FSM_Steady(1);
        ACTTIVE=FSM_Steady(2);
        SEND=FSM_Transient(1);
    }
    int pkCounter;
    cMessage *startStopBurst;
    cMessage *sendMessage;
    int numSent;
    int numReceived;

    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;
    public:
    Bursty();
    ~Bursty();

    protected:
    virtual void initialize()override ;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay()const override ;

    virtual void processTimer(cMessage *msg);
    virtual void processPacket(Packet *pk);
    virtual void generatePacket();
}

Define_Module(BurstyApp);

BurstyApp::BurstyApp()
{
    startStopBurst=sendMessage=nullptr;
}

BurstyApp::~BurstyApp()
{
    cancelAndDelete(startStopBurst);
    cancelAndDelete(sendMessage);
}

void BurstyApp::initialize()
{
    numSent=0;
    numReceived=0;
    WATCH(numSent);
    WATCH(numReceived);

    fsm.setName("fsm");

    destAddresses=cStringTokenizer(par("dsetAddresses").stdstringValue().c_str()).asInVector();

    myAddress=par("address");
    sleepTime=&par("sleepTime");
    burstTime=&par("burstTime");
    sendIATime=&par("sendIaTime");
    packetLengthBytes=&par("packetLength");

    endToEndDealySignal=registerSignal("endToEndDelay");
    hopCountSignal=registerSignal("hopCount");
    sourceAddressSignal=registerSignal("sourceAddresses");

    pkCounter=0;
    WATCH(pkCounter);

    startStopBurst=new cMessage("startStopBurst");
    sendMessage= new cMessage("sendMessage");

    scheduleAt(0,startStopBurst);
}

void BurstyApp::handleMessage(cMessag *msg)
{
    if(msg->isSelfMessage())
    {
        processTimer(msg);
    }
    else
    {
        processPacket(check_and_cast<Packet *>(msg));
    }
    
}

void BurstyApp::processTimer(cMessage *msg)
{
    simtime_t d;
    FSM_Switch(fsm)
    {
        case FSM_Exit(INIT):
            FSM_Goto(fsm,sleep);
            break;
        case Fsm_Enter(SLEEP):
            d=sleepTime->doubleValue();
            scheduleAt(simTime()+d,startStopBurst);

            EV<<"Sleeping for "<< d <<"s\n";
            bubble("burst ended ,sleeping ");
            getDisplayString().setTagArg("i",1,"");

            break;
        case FSM_Exit(SLEEP):
            d=burstTime->doubleValue();
            scheduleAt(simTime()+d,startStopBurst);

            EV<<"Starting burst if duration "<<d<<"s\n";
            bubble("burst sarted");
            getDisplayString().setTagArg("i",1,"yellow");
            if(msg!=startStopBusrt)
                throw cRuntimeError("invalid event in state ACTIVE");
            FSM_Goto(fsm,ACTIVE);
            break;
        case FSM_Enter(ACTIVE):
            d=sendIATime->doubleValue();
            EV<<"nexting sending in "<<d<<"s\n";
            scheduleAt(simTime()+d,sendMessage);
            break;
        case Fsm_Exit(ACTIVE):
            if(msg==sendMessage)
            {
                FSM_Goto(fsm,SLEEP);
            }
            else if(msg==startStopMessage)
            {
                cancelEvent(sendMessage);
                Fsm_Goto(fsm,SLEEP);
            }

            else
            {
                throw cRuntimeError("invaid event in sate ACTIVe");
            }
            break;
        case Fsm_Exit(SEND):
        {
            generatePacket();
            FSM_Goto(fsm,ACTIVE)；
            break;

        }
        
    }
}

void BustyApp::generatePacket()
{
    int destAddress=desAddresses[intuniform(0,destAddresses.size()-1];

    char pkname[40];
    sprintf(pkname,"pk-%d -to -%d ,#%d",myAddress,destAddresses,pkCounter++);

    Packet *pk=new Packet(pkname);
    pk->setBytesLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddresses);

    send(pk,"out");
}

void BustyApp::processPacket(Packet *pk)
{
    EV<<"Received packet "<<pk->getName()<<" after "<<pk->getHopCount() <<" hops \n";
    emit(endToEndDelaySignal,simTime()-pk->getCreationTime());
    emit(hopCountSignal,pk->getHopCount());
    emit(sourceAddressSignl,pk->getSrcAddr());
    numReceive++;
    delete pk;
}

void BurstyApp::refresDisplay()
{
    char txt[40];
    spintf(txt,"sent : %d received : %d",numSent,numReceived);
    getDisplayString().setTagArg("t",0,txt);
}