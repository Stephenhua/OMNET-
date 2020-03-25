/*
 * tic9.cc
 *
 *  Created on: 2018Äê10ÔÂ24ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;
class Tic8: public cSimpleModule
{
    private:
    simtime_t timeout;
    cMessage *timeoutEvent;
    public:
    Tic8();
    virtual ~Tic8();
    protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic8);
Tic8::Tic8()
{
    timeoutEvent = nullptr;
}

Tic8::~Tic8()
{
    cancelAndDelete(timeoutEvent);
}

void Tic8::initialize()
{
    timeout =1.0;
    timeoutEvent  = new cMessage("timeoutEvent");
    EV<<"Sending initial message\n";
    cMessage *msg= new cMessage("tictocMsg");
    send(msg,"out");
    scheduleAt(simTime()+timeout,timeoutEvent);
}

void Tic8::handleMessage(cMessage *msg)
{
    if(msg == timeoutEvent)
    {
        EV<<"Timeout expired ,resending message and restarting timer\n ";
        cMessage  *newMsg = new cMessage("tictocMsg");
        send(newMsg,"out");
        scheduleAt(simTime()+timeout,timeoutEvent);
    }
    else
    {
        EV<<"Timer canceled.\n";
        cancelEvent(timeoutEvent);
        delete msg;
        cMessage *newMsg= new cMessage("tictocMsg");
        send (newMsg,"out");
        scheduleAt(simTime()+timeout,timeoutEvent);

    }
}

class Toc8: public cSimpleModule
{
protected:
    virtual void handleMessage(cMessage *msg) override;

    };
Define_Module(Toc8);
void Toc8::handleMessage(cMessage *msg)
{
    if(uniform(0,1)<0.1)
     {
        EV<<"\"Losing\" message.\n";
        bubble("message lost");
        delete msg;
        }
    else
{
        EV<<"Sending back same message as acknowledge .\n";
        send(msg,"out");
}
}
