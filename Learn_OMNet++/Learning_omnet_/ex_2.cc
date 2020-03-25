/*
 * ex_2.cc
 *
 *  Created on: 2019Äê1ÔÂ19ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
using namespace omnetpp;

class Net : public cSimpleModule
{
private:
    int counter;
protected:
    virtual void initialize() override ;
    virtual void handleMessage( cMessage *msg) override ;
};

Define_Module(Net);

void Net::initialize()
{
    counter = par ("limit");
    if(par("sendMsgOnInit").boolValue()==true)
    {
        EV<<"Sending initial message\n";
        cMessage *msg=new cMessage("Hello");
        send(msg,"out");
    }
}

void Net::handleMessage(cMessage *msg)
 {
        counter--;
        if(counter==0)
        {
            EV<< getName()<<"'s  counter reached zero ,deleting message \n";

            delete msg;
        }
        else
        {
            EV<<getName()<<" 's counter is "<<counter <<"'s , sending back message \n";
            send(msg,"out");
            }
 }




