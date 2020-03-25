/*
 * ex13.cc
 *
 *  Created on: 2019Äê1ÔÂ25ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "ex13_m.h"

using namespace omnetpp;
class Txc_ex13:public cSimpleModule
{
private:
    long numSent;
    long numReceived;
    cLongHistogram hopCountStats;
    cOutVector hopCountVector;
protected:
    virtual TicTocMsg13 *generateMessage();
    virtual void forwardMessage(TicTocMsg13 *msg);
    virtual void initialize()override;
    virtual void handleMessage(cMessage *msg) override ;

    virtual void finish() override;
};

Define_Module(Txc_ex13);

void Txc_ex13 ::initialize()
{
    numSent = 0;
    numReceived =0;
    WATCH(numSent);
    WATCH(numReceived);

    hopCountStats.setName("hopCountStats");
    hopCountStats.setRangeAutoUpper(0,10,1.5);
    hopCountVector.setName("HopCount");

    if(getName()==0)
    {
        TicTocMsg13 *msg=generateMessage();
        scheduleAt(0.0,msg);
    }
}


TicTocMsg13 *Txc_ex13::generateMessage()
{
    int src =getIndex();
    int n=getVectorSize();
    int dest=intuniform(0,n-2);
    if(dest>=src)
    {
        dest++;
    }

    char msgname[20];
    sprintf(msgname,"tic-%d -toc-%d",src,dest);

    TicTocMsg13 *msg=new TicTocMsg13(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc_ex13::forwardMessage(TicTocMsg13 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);
    int n=gateSize("gate");
    int k=intuniform(0,n-1);

    EV<<"Forwarding message"<<msg<<" on gate ["<<k<<"]\n";
    send(msg,"gate$o",k);
}


void Txc_ex13::handleMessage(cMessage *msg)
{
    TicTocMsg13 *ttmsg=check_and_cast<TicTocMsg13 *>(msg);

    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount =ttmsg->getHopCount();
        EV<<"Message"<<ttmsg<<"arrived after "<<hopcount<<" hops.\n";
        bubble("Arrived ,starting new one!");

        numReceived++;
        hopCountVector.record(hopcount);
        hopCountStats.collect(hopcount);

        delete ttmsg;

        EV<<"Generating another message: ";
        TicTocMsg13 *newmsg=generateMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        forwardMessage(ttmsg);
    }
}

void Txc_ex13::finish()
{
    EV<<"Sent:          "<<numSent<<endl;
    EV<<"Received: "<<numReceived <<endl;
    EV<<"Hop count ,min:  "<<hopCountStats.getMin()<<endl;
    EV<<"Hop count,max:"<<hopCountStats.getMax()<<endl;
    EV<<"Hop count, mean:"<<hopCountStats.getMean()<<endl;
    EV<<"Hop count , stddev:"<<hopCountStats.getStddev()<<endl;

    recordScalar("#sent",numSent);
    recordScalar("#received",numReceived);

    hopCountStats.recordAs("hop count");
}


