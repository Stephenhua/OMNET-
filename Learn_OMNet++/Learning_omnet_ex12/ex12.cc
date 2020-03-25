/*
 * ex12.cc
 *
 *  Created on: 2019Äê1ÔÂ25ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "ex12_m.h"
using namespace omnetpp;

class Txc_ex12 : public cSimpleModule
{
private:
    long numSent;
    long numRecieved;
protected:
    virtual TicTocMsg12 *generateMessage();
    virtual void forwardMessage(TicTocMsg12 *msg);
    virtual void refreshDisplay() const override;

    virtual void initialize() override ;
    virtual void handleMessage(cMessage *msg) override ;
};


Define_Module(Txc_ex12);

void Txc_ex12::initialize()
{
    numSent=0;
    numRecieved =0;
    WATCH(numSent);
    WATCH(numRecieved);

    if(getIndex()==0)
    {
        TicTocMsg12 *msg =generateMessage();
        numSent++;
        scheduleAt(0.0,msg);
    }
}

void Txc_ex12::forwardMessage(TicTocMsg12 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);

    int n=gateSize("gate");
    int k=intuniform(0,n-1);

    EV<<"Forwarding message "<<msg<<" on gate["<<k<<"]\n";
    send(msg,"gate$o",k);
}

TicTocMsg12 *Txc_ex12::generateMessage()
{
    int src=getIndex();
    int n=getVectorSize();
    int dest=intuniform(0,n-2);
    if(dest>=src)
    {
        dest++;
    }
    char msgname[20];
    sprintf(msgname,"tic-%d-too-%d",src,dest);

    TicTocMsg12 *msg=new TicTocMsg12(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}
void Txc_ex12::handleMessage(cMessage *msg)
{
    TicTocMsg12 *ttmsg=check_and_cast <TicTocMsg12 *>(msg);
    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount=ttmsg->getHopCount();
        EV<<"Message"<<ttmsg<<" arrived after "<<hopcount <<" hops .\n";
        numRecieved++;
        delete ttmsg;
        bubble("Arrived , starting new one !");

        EV<<"Generating another message: ";
        TicTocMsg12 *newmsg=generateMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        forwardMessage(ttmsg);
    }
}

void Txc_ex12::refreshDisplay() const
{
    char buf[40];
    sprintf(buf,"rcvd: %ld sent: %ld",numRecieved,numSent);
    getDisplayString().setTagArg("t",0,buf);
}

