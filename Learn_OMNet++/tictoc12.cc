#include <omnetpp.h>
# include <stdio.h>
# include <string.h>

using namespace omnetpp;

# include "tictoc13_m.h"

messaage Tictoc12{
    int source;
    int destination;
    int hopcount;
}

class Txc13 public:cSimpleModule
{
    protected:
    virtual void initialize()override ;
    virtual void handleMessage(cMessage *msg) override ;
    virtual void forwardMessage(cMessage *msg) override;
    virtual TicToc13 *gengerateMessage();
}
Define_MOdule(Txc13);

void Txc13::initialize()
{
    if(getIndex()==0)
    {
        TicToc13 *msg=generateMessgae();
        scheduleAt(0.0,msg);
    }
}

void Txc13::handleMessage(cMessage *msg)
{
    TicToc13 *ttmsg=check_and_cast<TicToc13 *>(msg);
    if(ttmsg->getDistination()==getIndex())
    {
        EV<<"Message"<<ttmsg<<"arrived ,after "<<ttmsg->getHopcount<<"hops \n";
        bubble("Message ,arrived new one!\n");
        delete msg;

        EV<<"Generate another message:";
        TicToc13 *newmsg=gengerteMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
    }
    else
    {
        forwardMessage(ttmsg);
    }
    
}
TicTocMsg13 *Txc13::generateMessage()
{
    int src=getIndex();
    int n=getVectorSize();
    int des=intuniform(0,n-2);
    if(des>src+1)
    {
        des++
    }
    char msgname[30];

    sprintf(msgname,"%d-to -%d",src,dest);

    TicTocMsg13 *msg=new TicTocMsg13(msgname);

    msg->setSource(src);
    msg->setDestination(des);
    return msg;
}

void Txc13::forwardMessage(cMessage *msg)
{
    msg->getHopCount(msg->getHopCount()+1);
    
    int n=gateSize("gate");
    int k=intuniform(0,n-1);
    EV<<"Forwarding Message"<<msg<<" on gae ["<<k<<"]"<<endl;
    send2BS(msg,"gate$o",k);

}