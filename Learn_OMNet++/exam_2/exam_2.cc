/*
 * exam_2.cc
 *
 *  Created on: 2018Äê10ÔÂ28ÈÕ
 *      Author: Jason
 */
class BurstyGenerator:public cSimpleModule
{
protected:
int burstLength;
int burstCounter;
virtual void initialize();
virtual void handleMessage(cMessage *msg);
};
Denfine_Module(BurstyGenerator);
void BurstyGenerator::initialize()
{
    burstLength = par("burstLength");
burstCounter=burstLength;
scheduleAt(simTime(),new cMessage);
}
void BurstyGenerator ::handleMessage(cMessage *msg)
{
    cMessage *pkt=new cMessage;
send(pkt,"out");
if(--burstCounter==0)
{
    burstCounter = burstLength;
scheduleAt(simTime()+exponential(5.0),msg);
}
else
{
    scheduleAt(simTime()+exponential(1.0),msg);
}
}

int outGateId = findGate("outGate");
while(true)

{
    send (new cMessage ("job"),outGateId);
wait(5);
}
