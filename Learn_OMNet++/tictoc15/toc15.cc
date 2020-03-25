/*
 * toc15.cc
 *
 *  Created on: 2018Äê11ÔÂ1ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "tictoc15_m.h"

using namespace omnetpp;

class Txc15 :public cSimpleModule
{
private:
    long numSent;
    long numReceived;
    cLongHistogram hopCountStats;
    cOutVector hopCountVector;
protected:
    virtual TicTocMsg15 *generateMessage();
    virtual void forwardMessage(TicTocMsg15 *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Txc15);
void Txc15::initialize()
{
    numSent=0;
    numReceived =0 ;
    WATCH(numSent);
    WATCH(numReceived);

    hopCountStats.setName("hopCountStats");
    hopCountStats.setRangeAutoUpper(0,10,1.5);
    hopCountVector.setName("HopCount");

    if(getIndex()==0)
    {
        TicTocMsg15 *msg=generateMessage();
        scheduleAt(0.0,msg);
        }
}

void Txc15::handleMessage(cMessage *msg)
{
    TicTocMsg15 *ttmsg= check_and_cast<TicTocMsg15 *>(msg);
    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount =ttmsg->getHopCount();
        EV<<"Message" << ttmsg<<" arrived after "<<hopcount<<" hops.\n";
        bubble("ARRIVED ,starting new one!");

        numReceived++;
        hopCountVector.record(hopcount);
        hopCountStats.collect(hopcount);
        delete ttmsg;
        EV<<"Generating another message: ";
        TicTocMsg15 *newmsg = generateMessage();
        EV<< newmsg<<endl;
        numSent++;
    }
    else
    {
        forwardMessage(ttmsg);
    }

}

TicTocMsg15 *Txc15::generateMessage()
{
    int src=getIndex();
    int n=getVectorSize();
    int dest =intuniform(0,n-2);
    if(dest >= src)
    {
        dest++;
    }

    char msgname[20];
    sprintf(msgname,"tic-%d-to %d",src,dest);

    TicTocMsg15 *msg =new TicTocMsg15(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc15::forwardMessage(TicTocMsg15 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);

    int n=gateSize("gate");
    int k= intuniform(0,n-1);
    EV<<"Forwarding message "<<msg<<" on gate["<<k<<"]\n";
    send(msg,"gate$o",k);


}

void Txc15::finish()
{
    EV<<"Sent:         "<<numSent<<endl;
    EV<<"Received:    " <<numReceived <<endl;
    EV<<"Hop count,min:"<<hopCountStats.getMin()<<endl;
    EV<< "Hop count,max:"<<hopCountStats.getMean()<<endl;
    EV<<"Hop count,stddev: "<<hopCountStats.getStddev()<<endl;

    recordScalar("#sent",numSent);
    recordScalar("#received",numReceived);

    hopCountStats.recordAs("hop count");
}
