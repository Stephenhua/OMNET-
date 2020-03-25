/*
 * e6.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <omnetpp.h>
# include <string.h>

using namespace omnetpp;

class Tic6: public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;
public:
    Tic6();
    ~Tic6();
protected:
    virtual void initialize() override ;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic6);
Tic6::Tic6()
{
    timeoutEvent=nullptr;
}

Tic6::~Tic6()
{
    cancelAndDelete(timeoutEvent);
}

void Tic6::initialize()
{
    timeout=1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    EV<<"Sending initial message \n";
    cMessage *msg=new cMessage ("tictocMsg");
    send (msg,"out");
    scheduleAt(simTime()+timeout,timeoutEvent);

}

void Tic6::handleMessage(cMessage *msg)
{
    if(msg==timeoutEvent)
    {
        EV<<"Timeout expired ,resenting message and restarting timer \n";
        cMessage *newMsg=new cMessage("tictocMSg");
        send(newMsg,"out");
        scheduleAt(simTime()+timeout,timeoutEvent);
        }
    else
    {
        EV<<"Timer cancelled .\n";
        cancelEvent(timeoutEvent);
        delete msg;
        cMessage *newMsg =new cMessage("hello");
        send(newMsg,"out");
        scheduleAt(simTime()+timeout,timeoutEvent);
    }
}

class Toc6:public cSimpleModule
{
protected :
virtual void handleMessage(cMessage *msg)override;
};

Define_Module(Toc6);

void Toc6::handleMessage(cMessage *msg)
{
    if(uniform(0,1)<0.1)
      {
        EV<<"\"Losing \"message .\n";
        bubble("message lost");
        delete msg;
       }
    else
    {
        EV<<"Sending back same message as acknowledgment.\n";
        send (msg,"out");
    }


}


