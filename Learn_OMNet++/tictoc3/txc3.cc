/*
 * txc3.cc
 *
 *  Created on: 2018Äê10ÔÂ20ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class Txc3:public cSimpleModule
{
private:
   int counter;
protected:
virtual void initialize() override;
virtual void handleMessage(cMessage *msg)override ;
};

Define_Module(Txc3);

void Txc3::initialize()
{
    counter=10;
    WATCH(counter);
    if(strcmp("tic", getName())== 0)
    {
        EV<<"Sending initialize message\n";
        cMessage *msg = new cMessage("tictocMsg");
        send (msg,"out");
    }

  }

void Txc3::handleMessage(cMessage *msg)
{
    counter--;
if(counter==0)
{
    EV<<getName()<<"'s counter reached zero,deleting messag\n ";
     delete msg;
}
else
{
    EV<<getName()<<"'s counter is "<< counter <<"'s ,sending back message";
    send (msg,"out");
}
}
