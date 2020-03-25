# include <map>
# include <omnetpp.h>
# include "Packet_m.h"

using namespace omnetpp;

class Routing :public cSimpleModule
{
    private:
        int myAddress;
        typedef std::map<int ,int > RoutingTable;
        simSignal_t dorpSignal;
        simSignal_t outputIfSignal;

    protected:
        virtual void initialize() override ;
        virtual void handleMessage(cMessage *msg) override ;

};

Define_MOdule(Routing);

void Routing::initialize()
{
    myAddress=getParentModule()->par("address");

    dropSignal =registerSignal("drop");
    outputIfSignal= registerSignal("outoutIf");

    std::vector<std::string> nedType;
    cTopology *topo =new cTopology("topo");
    nedType.push_back(getParentModule()->getNedTypeName());

    topo->extractByNedTypeName(nedType);
    EV<<"cTopology found "<<topo->getNumNode()<<"nodes .\n";

    cTopology::Node *thisNode =topo->getNodeFor(getParentModule());

    for(int i=0;i<topo->getNumNodes();i++)
    {
        if(topo->getNode(i)==thisNode)
        {
            continue;
        }
        topo->caculateUnweightedSingleShortestPathsTo(topo->getNode(i));

        if(thisNode->getNumPaths()==0)
        {
            continue;
        }

        cGate *parentModuleGate =thisNode->getPath(0)->getLocalGate();

        int gateIndex =parentModuleGate->getIndex();
        int address = topo->getNode(i)->getModule()->par("address");

        rtable[address] =gateIndex;

        EV<<" towards address "<<address <<" gateIndex is "<<gateIndex <<endl;

    }
    delete topo;
}

void Routing::handleMessage(cMEssage *msg)
{
    Packet *pk = check_and_cast <Packet *>(msg);
    int destAddr =pk->getDestAddr();

    if(destAddr == myAddress)
    {
        EV<<" local delivery of packet "<< pk->getName() <<endl;
        send(pk,"localOut");
        emit(outputIfSignal,-1);
        return ;
    }

    RoutingTable ::iterator it=reable.find(destAddr);

    if(it == rtable.end())
    {
        EV<<" address "<<destAddr<<" unreachable ,discarding packet"<<pk->getName()<<endl;
        emit(dorpSignal,(long)pk->getByteLength());
        delete pk;
        return ;
    }

    int outGateIndex =(*it).second;
    EV<<"forwarding packet "<<pk->getName()<<" on gate index "<<outGateIndex<<endl ;

    pk->setHopCount(pk->getHopCount()+1);
    emit(outputIfSignal,outGateIndex);

    send(pk ," out ",outGateIndex);

    }