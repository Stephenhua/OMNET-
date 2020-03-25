# include<omnetpp.h>
# include <stdio.h>
# include <string.h>

using namespace omnetpp;

class Txc10 public:cSimpleModule
{
    protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override ;
    virtual void forrwardMessage(cMessage *msg);
}
Define_MOdule(Txc10);

void Txc10::initialize()
{
    if(getIndex==0)
    {
        char msgname[40];
        sprintf(msgname,"tic-%d",getIndex());
        cMessage *msg=new cMessage(msgname);
        scheduleAt(0.0,msg);
    }
    else
    {
        forwardMessage(msg);
    }
    
}

void Txc10::handleMessage(cMessage *msg)
{
    if(getIndex()==3)
    {
        EV<<"Message"<< msg <<"arrived \n";
        delete msg;
    }
    else
    {
        forrwardMessage(msg);
    }

    
}

void forwardMessage(cMessag *msg)
{
    int n=gateSize("out");
    int k=intuniform(0,n-1);
    EV<<"Forwarding Message "<<msg<<"arrived on port out ["<<k<<"].\n";
    send(msg,"out",k);
}