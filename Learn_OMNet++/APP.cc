# include <vector>
# include <omnetpp.h>
# include "packet_m.h"
# include <iostream>
using namespace std;
packet Packet
{
    int srcAddr;
    int destAddr;
    int hopCount;
}
using namespace omnetpp;

class App::public cSimpleModule
{
    private:
     int address;
     vector<int > destAddresses;
     cPar *sendIATime;
     cPar *packetLengthBytes;


     cMessage *generatePacket;
     long pkCounter;

     simsignal_t endToEndDealySignal;
     simsignal_t hopCountSignal;
     simsignal_t sourceAddressSignal;

     public:
     App();
     virtual ~App();
     protected:
     virtual void initialize() override ;
     virtual void handleMessage(cMessage *msg) override;

}
Define_Module(App);

App::App()
{
    generatePacket=nullpter;
}

App::~App()
{
    cancelAndDelete(generatePacket);
}


void App::initialize()
{
    myAddress=par("address");
    packetLengthBytes=&par("pakcetLength");
    sendIATime=&par("sendIaTime");
    pkCounter=0;

    WATCH("pkCounter");
    WATch(myAddress);

    const char *destAddressesPar=par("destAddresses");
    cStringTokenizer tokenizer(destAdderessesPar);
    const char *token;
    while((token=toeknizer(destAddressesPar))!=nullptr)
    {
        destAddresses.push_back(token);
    }

    if(destAddresses.size()=0)
    {
        throw CRunTimeError("At least one addresss must be in the destination programing");
    }

    genreatePacket=new cMessage("nextPacket");
    scheduleAt(sendIATime()->double,generatePacket);

    endToEndDealySignal=registerSignal("endToEndDelay");
    hopCountSignal=registerSignal("hopCount");
    sourceAddressSignal=registerSignal("sourceAddress");
}

void App::handleMessage(cMessage *msg)
{
    if(msg==generatePacket)
    {
        int destAddress=destAddresses[intuniform(0,destAddresses.size()-1)];

        char pkname[40];
        sprintf(pkname," %d,to ,%d ",myAdderss,destAddress,pkCounter++);
        EV<<"Generating packet "<<pkname<<endl;

        Packet *pk=new Packet(pkname);
        pk->setByteLenght(packetLengthBytes->intValue());
        pk->setKind(intUniform(0,7));
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);

        send(pk,"out");
        
        scheduleAt(simTime()+sendIATime->doubleValue(),generatePacket);

        if(hasGUI())
        {
            getParentModule()->bubble("Generating packet..!\n");
        }
    }
    else
    {
        Packet *pk=check_and_cast<Packet *>(msg);
        EV<<"received"<<pk->getName()<< " after " <<pk->getHopCount()<<" hops"<<endl;
    emit(endToEndDealySignal,simTime()-pk->getCreatinTime());
    emit(hopCountSignal,pk->getHopCount());
    emit(sourceAddressSignal,pk->getSourceAddress());
    delete pk;

    if(hasGUI())
    {
        getParentModule()->bubble("Arrived !");
    }
     }
}