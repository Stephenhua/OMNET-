/*
 * tic9.cc
 *
 *  Created on: 2018Äê10ÔÂ24ÈÕ
 *      Author: Jason
 */
# include<stdio.h>
# include <string.h>
# include<omnetpp.h>

using namespace omnetpp;
class Tic9:public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    int seq;
    cMessage *message;
public:
    Tic9();
    virtual ~Tic9();
protected:
    virtual cMessage *gengerateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override ;

};

Define_Module(Tic9);

Tic9::Tic9()
{
    timeoutEvent = message = nullptr;
}
Tic9::~Tic9()
{
    cancelAndDelete(timeoutEvent);
delete message;
}
void Tic9::initialize()
{
    seq=0;
timeout =1.0;
timeoutEvent = new cMessage("timeoutEvent");
EV<<"Sending initial message\n";
message = gengerateNewMessage();
sendCopyOf(message);
scheduleAt(simTime()+timeout,timeoutEvent);
}

void Tic9::handleMessage(cMessage *msg)
{
    if(msg==timeoutEvent)
    {
        EV<<"Timeout expired ,resending message and restarting timer \n";
        sendCopyOf(message);
        scheduleAt(simTime()+timeout,timeoutEvent);
    }
    else
    {
        EV<<"Received"<<msg->getName()<<"\n";
        delete msg;
        EV<<"Timer cancelled.\n";
        cancelEvent (timeoutEvent);
        delete message;
        message=gengerateNewMessage();
        sendCopyOf(message);
        scheduleAt(simTime()+timeout,timeoutEvent);
    }
    }
cMessage *Tic9::gengerateNewMessage()
{
    char msgname[20];
    sprintf(msgname,"tic-%d",++seq);
    cMessage *msg=new cMessage(msgname);
     return msg;
}
void Tic9::sendCopyOf(cMessage *msg)
{
    cMessage *copy=(cMessage *)msg->dup();
    send(copy,"out");
}

class Toc9:public cSimpleModule
{
    protected:
    virtual void handleMessage(cMessage *msg) override ;

};

Define_Module(Toc9);

 void Toc9::handleMessage(cMessage *msg)
{
    if(uniform(0,1)<0.1)
{
        EV<<"\"Losing \"message "<<msg<<endl;
        bubble("message lost");
        delete msg;
}
    else
{
        EV<<msg<<" received ,sending back an acknowledege.\n";
        delete msg;
       send(new cMessage ("ack"),"out");
}
}

