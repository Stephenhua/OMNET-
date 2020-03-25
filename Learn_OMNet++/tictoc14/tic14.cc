/*
 * tic14.cc
 *
 *  Created on: 2018Äê10ÔÂ28ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "tictoc14_m.h"
using namespace omnetpp;

class Txc14 :public cSimpleModule
{
private:
long numSent;
long numReceived;
protected:
virtual TicTocMsg14 *generateMessage();
virtual void forwardMessage(TicTocMsg14 *msg);
virtual void refreshDisplay() const override ;
virtual void initialize() override;
virtual void handleMessage(cMessage *msg) override ;
};

void Txc14::initialize()
{
    numSent =0;
    numReceived =0;
    WATCH(numSent);
    WATCH(numReceived);

if(getIndex()==0){
    TicTocMsg14 *msg = generateMessage();
    numSent++;
    scheduleAt(0.0, msg);
}
}

void Txc14::handleMessage(cMessage *msg)
{
    TicTocMsg14 *ttmsg =check_and_cast <TicTocMsg14 *> (msg);
    if(ttmsg->getDestination()== getIndex()){
        int hopcount = ttmsg->getHopCount();
        EV<<"Message"<<ttmsg<<" arrived after"<<hopcount<<" hopes\n";
        numReceived++;
        delete ttmsg;
        bubble("ARRIVED m starting new one!");

        EV<<" Generating another message: ";
        TicTocMsg14 *newmsg=generateMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        forwardMessage(ttmsg);
    }
}

TicTocMsg14 *Txc14::generateMessage()
{
    int src= getIndex();
int n=getVectorSize();
int dest= intuniform(0,n-2);
if(dest>=src)
{dest++;}

char msgname[20];
sprintf(msgname,"tic- %d -to-%d",src,dest);
TicTocMsg14 *msg= new TicTocMsg14(msgname);
msg->setSource(src);
msg->setDestination(dest);
return msg;
}

void Txc14::forwardMessage(TicTocMsg14 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);
    int n= gateSize("gate");
    int k=intuniform(0,n-1);
    EV<<"Forwarding message"<<msg<<" on gates"<<k<<"]\n";
    send(msg, "gate$o",k);
}

void Txc14::refreshDisplay() const
{
    char buf[40];
    sprintf(buf," rcvd: %1d sent: %1d",numReceived,numSent);
    getDisplayString().setTagArg("t",0,buf);
}


