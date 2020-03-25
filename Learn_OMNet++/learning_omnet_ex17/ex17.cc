/*
 * ex17.cc
 *
 *  Created on: 2019年2月21日
 *      Author: Jason
 */
# include <omnetpp.h>
using namespace omnetpp;

class HelloModule : public cSimpleModule
{
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    };

Define_Module(HelloModule);

void HelloModule::initialize()
{
    EV<<"Hello ,world!\n";
}

void HelloMpdule::handleMessage(cMessage *msg)
{
    delete msg;
}


class FooProtocol : public cSimpleModule
{
protected:
    virtual void processMsgFromHigherLayer(cMessage *packet);
    virtual void processMsgFromLowerLayer(FooPacke *packet);
    virtual void processTimer(cMessage *timer);

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

void FooProtocol::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
        processTimer(msg);
    else if(msg->arriveedOn ("fromNetw"))
        processMsgFromLowerLayer(check_and_cast<FooPacket*>(msg));
    else
        processMsgFromHigherLayer(msg);
}

class Generator :public sSimpleModule
{
public:
    Generator():cSimpleModule() {}
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    };


Define_Module(Generator);

void Generator ::initialize()
{
    scheduleA他(simTime(),new cMessage);
}

void Generator::handleMessage(cMessage *msg)
{
    cMessaeg *pkt =new cMessage;
    send(pkt,"out");
    scheduleAt(simTime()+exponential(1.0),msg);
}

class BurstyGenerator :public cSimpleModule
{
protected:
    int burstLength;
    int burstCounter;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    };

Define_Module(BurstyGenerator);

void BurstyGenerator::initialize()
{
    burstLength=par("burstLength");
    burstCounter=burstLength;
    scheduleAt(simTime(),new cMessage);
}

void BurstyGenerator::handleMessage(cMessage *msg)
{
    cMessage *pkt=new cMessage;
    send(pkt,"out");
    if(--burstCounter==0)
    {
        burstCounter =burstLength;
        scheduleAt(simTime()+exponential(5.0),msg);
    }
    else
    {
        scheduleAt(simeTime()+exponential(1.0),msg);
    }
}

class Source :public cSimpleModule
{
protected:
    long numJobs;
    virtual void initialize();
    };

void Source::initialize()
{
        numJobs=par("numsJobs");
}

void Source::handleMessage(cMessage *msg)
{
    scheduleAt(simTime()+par("interval").doubleValue(),timerMsg);
    int n=getNumParams();
    for(int i=0;i<n;i++)
    {
        cPar& p=par(i);
        EV<<"parameter:"<<p.getName()<<"\n";
        EV<<" type:"<<cPar::getTypeName(p.getType())<<"\n";
        EV<<" contains:"<<p.str()<<"\n";
    }

    const char *vstr =par("v").stringValue();
    std::vector<std::string> v=cStringTokenizer(vstr).asVector();

    const char *vstr="34 45 13 46 72 41";
    std::vector<int> v=cStringTokenizer().asIntVector();

    const char *vstr ="0.4311 -0.7402 0.7134";
    std::vector<double> v=cStringTokenizer().asDoubleVector();

    const char *str ="3.25 1.83 34 x 19.8";
    std::vector <double > result;
    cStringTokenizer tokenizer(str);

    while(tokenizer.hasMoreTakens())
    {
        const char *token=tokenizer.nextToken();
        if(strcmp(token,"X")==0)
        {
            result.push_back(DEFAULT_VALUE);
        }
        else
        {
            result.push_back(atof(token));
        }
    }


    //About cGate
    cGate *outGate =gate ("out");
    cGate  *gIn =gateHalf("g",cGate::INPUT);
    cGate  gOut= gateHalf("g",cGate::OUTPUT);

    if(hasGate("optout"))
        send(new cMessage(),"optout");

    int gateId= gate("in")->getId();
    int gateId= findGate("in");

    for(int i=0;i<gateSize("out");i++)
    {
        cGate *gate = gate("out",i);
    }

    int baseId =gateBaseId("out");
    int size= gateSize("out");
    for(int i=0;i<size;i++)
    {
        CGate *gate =gate(baseId+i);
    }

    for(cModule::GateIterator i(this);!i.end();i++)
    {

        cGate *gate=*i;
        EV<<gate->getFullName()<<":";
        EV<<"id="<<gate->getId()<<" ,";
        if(!gate->isVector())
            EV<<"scalar gate, ";
        else
            EV<<"gate "<<gate->getIndex()<<" in vector "<<gate->getName()<<" of size"<<gate->getVectorSize()<<" .";
        EV<<"type:"<<cGate::getTypeName(gate->getType());
        EV<<"\n";
    }

    cGate *gate =gate("somegate");
    cGate *otherGate = gate->getype()==cGate::OUTPUT?gate->getNextGate():gate->getPreviousGate();
    if(otherGate)
        EV<<"gate is connected to :"<<otherGate->getFullPath()<<endl;
    else
        EV<<"gate not connected "<<endl;


    cChannel *channel = gate->getChannel();
    cGate *gate =channel->getSourceGate();

    scheduleAt(absoluteTime,msg);
    scheduleAt(simTime()+delta,msg);

   send(cMessage *msg,const char *gateName,int index=0);
   send(cMessage *msg,in gateId);
   send(cMessage *msg,cGate *gate);
   send(msg,"out");
   send (msg,"outv",i);
   send(msg,"g$o");
   send(msg,"g$o",i);

   int outGateBaseId =gateBasedId("out");
   for(int i=0;i<n;i++)
   {
       send(i=n-1?msg:msg->dup(),outGateBaseId+i);
   }

   cMessage *copy=packet->dup();
   send(copy,"out");
   delete packet;
}




void Protocol ::handleMessage(cMessage *msg)
 {
       if(msg==timeoutEvent)
       {
           send(currentPacket->dup(),"out");
           scheduleAt(simTime()+timeout,timeoutEvent);
       }
       else if
       {
           cancelEvent(timeoutEvent);
       }
   }



















































































