/*
 * exercise_1.cc
 *
 *  Created on: 2019Äê1ÔÂ14ÈÕ
 *      Author: Jason
 */
# include<stdio.h>
# include <omnetpp.h>
# include <string.h>

using namespace omnetpp;

class Tcx1:public cSimpleModule
{
protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg)override;

};

Define_Module (Tcx1);

void  Tcx1::initialize()
{
    if(getIndex()==0)
    {
        char msgname[20];
        sprintf(msgname,"tic-%d",getIndex());
        cMessage *msg=new cMessage(msgname);
        scheduleAt(0.0,msg);
    }

}

void Tcx1::handleMessage(cMessage *msg)
{
    if(getIndex()==3)
    {
        EV<<"Message"<<msg<<"arrived.\n";
        delete msg;
    }
    else
    {
        forwardMessage(msg);
    }
}

void Tcx1::forwardMessage(cMessage *msg)
{
    int n=gateSize("out");
    int k=intuniform(0,n-1);
    EV<<"Forwarding message"<<msg<<"on port out ["<<k<<"]\n";
    send(msg,"out",k);

}


