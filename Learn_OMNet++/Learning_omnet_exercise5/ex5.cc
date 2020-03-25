/*
 * ex5.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */

#  include <iostream>
# include <stdio.h>
# include <omnetpp.h>
# include <string.h>

using namespace omnetpp;

class Ex5:public cSimpleModule
{
private:
cMessage *event;
cMessage *tictocMsg;
public:
Ex5();
virtual ~Ex5();
protected:
virtual void initialize() override ;
virtual void handleMessage(cMessage *msg) override ;
};

Define_Module(Ex5);

Ex5::Ex5()
{
    event=tictocMsg= nullptr;
 }
Ex5::~Ex5()
{
    cancelAndDelete(event);
     delete tictocMsg;
 }

void Ex5::initialize()
{
    event =new cMessage("Hello,2019");
    tictocMsg=nullptr;
    if(strcmp("tic",getName())==0)
    {
        EV<<"Scheduling first send to t=5.0s\n";
        scheduleAt(10.0,event);
        tictocMsg=new cMessage("hello,2018");
    }
}

void Ex5::handleMessage(cMessage *msg)
{
    if(msg=event)
    {
        EV<<"waiting period is over ,sending back message \n";
        send(tictocMsg,"out");
        tictocMsg=nullptr;
    }
    else
    {
        if(uniform(0,1)<0.1)
        {
            EV<< "\"Losing\" message\n";
            delete msg;
        }
        else
        {
            simtime_t delay=par("delayTime");
            EV<<"Message arrived ,starting to wait "<<delay<<" secs ...\n";
            tictocMsg=msg;
            scheduleAt(simTime()+1.0,event);
        }
    }
    }


