/*
 * ex9.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;

class Txc_ex9 :public cSimpleModule
{
protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override ;
    virtual void handleMessage(cMessage *msg) override ;
};
Define_Module(Txc_ex9);

void Txc_ex9::initialize()
{
    if(getIndex()==0)
    {
        char msgname[20];
        sprintf(msgname,"tic-%d",getIndex());
        cMessage *msg=new cMessage(msgname);
        scheduleAt(0.0,msg);
    }
}

void Txc_ex9::handleMessage(cMessage *msg)
{
    if(getIndex()==3)
    {
        EV<<"Message "<<msg <<"Arrived.\n";
        delete msg;
    }
    else
    {
        forwardMessage(msg);
    }
}
void Txc_ex9::forwardMessage(cMessage *msg)
{
    int n = gateSize("out");
    int k=intuniform(0,n-1);
    EV<<"Forwarding message "<<msg<<" on port out[ "<<k<<" ].\n";
    send(msg,"out",k);
    }
