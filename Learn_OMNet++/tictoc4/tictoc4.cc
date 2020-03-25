/*
 * ticto4.cc
 *
 *  Created on: 2018Äê10ÔÂ21ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;

class Txc4:public cSimpleModule
{
private:
     int counter;
protected:
     virtual void initialize() override;
     virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Txc4);
void Txc4::initialize()
{
    counter=par("limit");
    if(par("sendMsgOnInit").boolValue()==true)
    {
        EV<<"Sending initial message\n";
        cMessage *msg=new cMessage("tictocMsg");
        send(msg,"out");
    }
}
void Txc4::handleMessage(cMessage *msg)
{
    counter--;
    if(counter==0)
    {
        EV<<getName()<<"'a couter reached zaro,deleting message\n";
        delete msg;
    }
    else
    {
        EV<<getName()<<"'s counter is "<<counter<<"'s ,sending back message\n";
        send (msg,"out");
    }


}


