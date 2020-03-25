/*
 * tic6.cc
 *
 *  Created on: 2018Äê10ÔÂ21ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;

class Txc6:public cSimpleModule
{
private:
     cMessage *event;
     cMessage *tictocMsg;
public:
     Txc6();
     virtual ~Txc6();
protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc6);
Txc6::Txc6()
{
    event =tictocMsg=nullptr;
}
Txc6::~Txc6()
{
    cancelAndDelete(event);
    delete tictocMsg;
}

void Txc6::initialize()
{
    event =new cMessage("event");
    tictocMsg=nullptr;
    if (strcmp("tic",getName()) == 0)
    {
        EV<<"Scheduling first send to t=5.0s\n";
        tictocMsg=new cMessage("tictocMsg");
        scheduleAt(5.0,event);
    }

}


void Txc6::handleMessage(cMessage *msg)
{
    if(msg == event)
    {
        EV<<"Wait period is over ,sending back message\n";
        send(tictocMsg,"out");
        tictocMsg =nullptr;
    }
    else
    {
        EV<<"Message arrived ,starting to wait 1 sec ..\n";
        tictocMsg= msg;
        scheduleAt(simTime()+1.0,event);
    }
 }
