/*
 * e3.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */
#  include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;

class txc3 :public cSimpleModule
{
private:
    int counter;
public:
    virtual void initialize() override ;
    virtual void handleMessage(cMessage *msg) override ;
    };

Define_Module(txc3);
void txc3 ::initialize()
{
    counter=par("limit");
    if(par("sendMsgOnInit").boolValue()==true)
    {
        EV<<"Sending initial message\n";
        cMessage *msg=new cMessage("Hello");
        send(msg,"out");
    }
}

void txc3::handleMessage(cMessage *msg)
{
    counter--;
    if(counter==0)
    {
        EV<<getName()<<"'s counter reached zero,deleting message\n";
        delete msg;
    }
    else
    {
        EV << getName() <<'s counter is '<< counter <<",sending back message \n";
        send(msg,"out");
    }
}


