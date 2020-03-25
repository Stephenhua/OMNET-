/*
 * e4.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */

# include <string.h>
# include <stdio.h>
# include <omnetpp.h>

using namespace omnetpp;

class Tic_ex4: public cSimpleModule
{
private:
    cMessage *event;
    cMessage *tictocMsg;
public:
    Tic_ex4();
    virtual ~Tic_ex4();
protected:
    virtual void initialize() override ;
    virtual void handleMessage (cMessage *msg) override;

};

Define_Module(Tic_ex4);

Tic_ex4::Tic_ex4()
{
    event=tictocMsg=nullptr;
    }

Tic_ex4::~Tic_ex4()
{
    cancelAndDelete(event);
    delete tictocMsg;
}

void Tic_ex4::initialize()
{
    event=new cMessage(" Hello  ");
    tictocMsg=nullptr;

    if(strcmp("tic_1",getName()) == 0)
    {
        EV<<"Scheduling first send to t=1.0s\n";
        tictocMsg=new cMessage("hello , toc_1");
        scheduleAt(5.0,event);
    }
}

void Tic_ex4::handleMessage(cMessage *msg)
{
   if(msg==event)
   {
       EV<<" wait period is over,sending  back message \n";
       send(tictocMsg,"out");
       tictocMsg=nullptr;
   }
   else
   {
       EV<<"Message arrived , starting to wait  a  sec ... \n";
       tictocMsg=msg;
       scheduleAt(simTime()+1.0,event);
   }
}
