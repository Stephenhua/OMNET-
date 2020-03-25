/*
 * e10.cc
 *
 *  Created on: 2019Äê1ÔÂ20ÈÕ
 *      Author: Jason
 */
# include <string.h>
# include <omnetpp.h>
# include <stdio.h>

using namespace omnetpp;

class Txc_ex10 :public cSimpleModule
{
protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override ;
    virtual void handleMessage (cMessage *msg) override;

};

Define_Module (Txc_ex10);

void Txc_ex10::initialize()
{
    if(getIndex()==0)
    {
        char msgname[20];
        sprintf(msgname, "t-%d",getIndex());
        cMessage *msg =new cMessage(msgname);
        scheduleAt(0.0,msg);
    }
}


void Txc_ex10::handleMessage(cMessage *msg)
{
    if(getIndex()==5)

    {
        EV<<"Message "<<msg<<" arrived .\n";
        delete msg;
    }
    else
    {
        forwardMessage (msg);
    }

}


void  Txc_ex10::forwardMessage(cMessage *msg )
{
    int n=gateSize("gate");
    int k=intuniform(0,n-1);
    EV<<"Forwarding message "<<msg <<" on gate ["<<k<<" ] .\n";
    send(msg,"gate$o",k);
}

