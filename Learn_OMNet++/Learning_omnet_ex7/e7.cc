/*
 * e7.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */
#  include <omnetpp.h>
# include <string.h>
#  include <stdio.h>

using namespace omnetpp;

class Tic7 :public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    int seq;
    cMessage *message;
public:
    Tic7();
    ~Tic7();
protected:
    virtual cMessage *generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize() override ;
    virtual void handleMessage(cMessage *msg) override ;
};

Define_Module(Tic7);

Tic7::Tic7()
{
    timeoutEvent=message=nullptr;
}

Tic7::~Tic7()
{
    cancelAndDelete(timeoutEvent);
     delete message;
}

void Tic7::initialize()
{
    seq=0;
    timeout=1.0;
    timeoutEvent=new cMessage("Hello ,Are you receiving my message\n");
     EV << "Sending initialize message\n";
     message = generateNewMessage();
     sendCopyOf(message);
     scheduleAt(simTime()+timeout,timeoutEvent);
}

void Tic7::handleMessage(cMessage *msg)
{
    if(msg==timeoutEvent)
{
        EV<<"Timeout expired,resending message and restarting timer\n ";
        sendCopyOf(message);
        scheduleAt(simTime()+timeout,timeoutEvent);

}
    else
    {
        EV<<"Received:"<<msg->getName()<<"\n";
        delete msg;
        EV<<"Timer cancelled .\n";
        cancelEvent(timeoutEvent);
        delete message;
    }

}

cMessage *Tic7::generateNewMessage()
{
    char msgname[20];
    sprintf(msgname,"tic-%d",++seq);
    cMessage *msg= new cMessage(msgname);
    return msg;
}

void Tic7::sendCopyOf(cMessage *msg)
{
    cMessage *copy=(cMessage *) msg->dup();
    send(copy,"out");
}


class Toc7:public cSimpleModule
{
   protected:
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Toc7);

void Toc7::handleMessage(cMessage *msg)
{
    if(uniform(0,1)<0.1)
    {
        EV<<"\"Losing\"message"<<msg<<endl;
        bubble ("message lost");
        delete msg;
    }
    else
    {
        EV<< msg<<" received ,sending back an knowledgement.\n";
        delete msg;
        send (new cMessage("ack"),"out");
    }

}

