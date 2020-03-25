/*
 * ex15.cc
 *
 *  Created on: 2019Äê2ÔÂ18ÈÕ
 *      Author: Jason
 */

# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "ex15_m.h"

using namespace omnetpp;

class Txc_ex15 :public cSimpleModule
{
private:
simsignal_t arrivalSignal;
protected:
    virtual TicTocMsg_ex15 *generateMessage();
    virtual void forwardMessage(TicTocMsg_ex15 *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override ;

};

Define_Module(Txc_ex15);
void Txc_ex15 ::initialize()
{
    arrivalSignal =registerSignal("arrival");
    if(getIndex()==0)
    {
        TicTocMsg_ex15 *msg=generateMessage();
        scheduleAt(0.0,msg);
    }

}


void Txc_ex15::handleMessage(cMessage *msg)
{
    TicTocMsg_ex15 *ttmsg=check_and_cast<TicTocMsg_ex15 *>(msg);
    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount=ttmsg->getHopCount();
        emit(arrivalSignal,hopcount);

        if(hasGUI())
        {
            char label[50];
            sprintf(label,"last hopCount = %d",hopcount);

            cCanvas *canvas =getParentModule()->getCanvas();
            cTextFigure *textFigure = check_and_cast<cTextFigure*>(canvas->getFigure("lasthopcount"));
            textFigure->setText(label);

        }

        EV<<"Message"<<ttmsg<<" arrived after "<<hopcount<<"hops.\n";
        bubble("Arrived ,starting new one!");
        delete ttmsg;

        EV<<"Generating another message:";
        TicTocMsg_ex15 *newmsg= generateMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
    }
    else
    {
        forwardMessage(ttmsg);
    }

}


TicTocMsg_ex15 *Txc_ex15::generateMessage()
{
    int src= getIndex();
    int n=getVectorSize();
    int dest=intuniform(0,n-2);
    if(dest>=src)
        dest++;
    char msgname[20];
    sprintf(msgname,"tic-%d-to-%d",src,dest);

    TicTocMsg_ex15 *msg=new TicTocMsg_ex15(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}


void Txc_ex15::forwardMessage(TicTocMsg_ex15 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);
    int n=gateSize("gate");
    int k=intuniform(0,n-1);
    EV<<"Forwarding message"<<msg<<" on gate["<<k<<"]\n";
    send (msg,"gate$o",k);
}
