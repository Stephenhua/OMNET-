/*
 * tict12.cc
 *
 *  Created on: 2018Äê10ÔÂ25ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;
class Txc12 :public cSimpleModule
{
protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    };
Define_Module(Txc12);

void Txc12::initialize()
{
    if(getIndex()==0)
    {
        char msgname[20];
        sprintf(msgname," tic-%d",getIndex());
        cMessage *msg=new cMessage(msgname);
        scheduleAt(0.0,msg);
    }
}


void Txc12::handleMessage(cMessage *msg)
 {
    if(getIndex()== 3)
    {
        EV<<" Message "<<msg<<" arrived .\n";
        delete msg;
    }
    else
    {
        forwardMessage(msg);
    }
}

void Txc12::forwardMessage(cMessage *msg)
{
    int n = gateSize("gate");
int k = intuniform(0,n-1);

EV<<" Forwarding message "<<msg<<" on gate ["<<k<<"] \n";

send (msg, " gate$o",k);
}





















