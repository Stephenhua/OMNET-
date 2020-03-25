# include <iostream>
# include <string.h>
# include <omnetpp.h>
# include "tictoc15_m.h"
using namespace omnetpp;

class Txc15 public: cSimpleModule
{
    private:
    long numSent;
    long numReceived;
    cHistogram hopCountState;
    CoutVector hopCountVector;

    protected:
    void TicToc15 *generateMessage();
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) ;
    virtual void forwardMessage(cMessage *msg) override;
    virtual void finish()override;


};

Define_Module(Txc15)

void Txc15::initialize()
{
    numSent=0;
    numReceived=0;
    WATCH(numSent);
    WATCH(numReceived);

    hopCountState.setName("hopCountState");
    hopCountVector.setName("hopCount");

    if(getIndex()==0)
    {
        TicToc15 *msg=check_and_cast<TicToc15 *>(msg);
        EV<<"This is start\n";
        scheduleAt(0.0,msg);
    }
}

void Txc15::handleMessage(cMessage *msg)
{
    TicToc15 *ttmsg=new TicToc15(msg);
    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount=ttmsg-getHopCount();
        EV<<"message "<<ttmsg<<"arrived after"<< hopcount()<<"hops\n";
        bubble("Arrived ,starting new one ");

        numReceived++;
        hopCountVector.record(hopcount);
        hopCountState.collect(hopcount);

        delete ttmsg;
        EV<<"Generating new message :\n";
        TicToc15 *newmsg=generateMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        forwardMessage(msg);
    }
    
}

TicToc15 * Txc15::generateMessage()
{
    int src=getIndex();
    int n=getVectorSize();
    int dest=intuniform(0,n-2);
    if(dest>=src)
    {
        dest++;
    }

    char msgname[40];
    sprintf(msgname,"%d to %d ",src,dest);
    TicToc15 *msg=new TicToc15(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc15::forwardMessage(TicToc15 *msg)
{
    msg->setHopCount(msg->getHopCoount()+1);
    int n=gateSize("gate");
    int k=intuniform(0,n-2);
    EV<<"Forwarding message "<<msg<<"on gate ["<<k<<"]\n";
    send(msg,"gate$o",k);
}

void Txc15::finish()
{
    EV<<"Sent    : "<<numSent<<endl;
    EV<<"Received :"<<numReceived<<endl;
    EV<<"HopCount ,min:"<<hopcountState.getMin()<<endl;
    EV<<"HopCount ,max:"<<hopCountState.getMax()<<endl;
    EV<<"HopCount ,mean"<<hopCountState.getMean()<<endl;
    EV<<"HopCount ,stddev"<<hopCountState.getStddev()<<endl;

    recordScalar("#sent",numSent);
    recordScalar("#received",numReceived);
    hopCountState.recordAs("hop Count");

}
