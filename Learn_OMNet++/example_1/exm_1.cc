/*
 * exm_1.cc
 *
 *  Created on: 2018Äê10ÔÂ27ÈÕ
 *      Author: Jason
 */
# include <omnetpp.h>

class FFGenerator:public cSimpleModule
{
private:
cMessage *sendMessageEvent;
long numeSent;
public:
FFGenerator();
virtual ~FFGenerator();
protected:
virtual void initilize();
virtual void handleMessage(cMessage *msg);
virtual void finish();
};

# include "FFGenerator.cc"

Define_Module(FFGenerator);
FFGenerator ::FFGenerator()
{
    sendMessageEvent = NULL;
}
void FFGenerator::initialize()
{
    numSent =0;
sendMessageEvent = new cMessage (" sendMessageEvent");
scheduleAt(0.0,sendMessageEvent);
}

void FFGenerator::handleMessage(cMessage *msg)
{
    ASSERT(msg== snedMessageRvent);
cMessage *m = new cMessage("packet";)
m->setLength(par("msgLength"));
send(m,"out");
numSet++;
double deltaT=(double )par("sendIaTime");
scheduleAt(simTime()+deltaT,sendMessageEvent);
}

void FFGenerator::finish()
{
    recordScalar("packets sent",numSent);
}

FFGenerator::~FFGenerator()
{
    cancelAndDelete(sendMessageEvent);
}



