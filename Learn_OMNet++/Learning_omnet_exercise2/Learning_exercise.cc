/*
 * Learning_exercise.cc
 *
 *  Created on: 2019��1��19��
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;

class Txc3 : public cSimpleModule
{
private:
    int counter;
protected:
    virtual void initialize() override ;
    virtual void handleMessage(cMessage *msg) override ;
};

Define_Module(Txc3);

void Txc3::initialize()
{
    counter = 10;

    WATCH(counter);

    if(strcmp("tic",getName())==0)//strucmp compare A and B if equal
    {
        EV<<"Sending initial message \n";
    cMessage *msg=new cMessage("tictocMessage");
    send (msg,"out");
    }
}

void Txc3::handleMessage(cMessage *msg)
{
    counter--;
    if(counter==0)
    {
        EV << getName() << "'s counter reached zero, deleting message \n";
        delete msg;

    }
    else
    {
        EV << getName() <<"'s couter is "<<counter <<", sending back message\n";
        send (msg,"out");

    }
}