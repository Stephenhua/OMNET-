/*
 * tic16.cc
 *
 *  Created on: 2018Äê11ÔÂ7ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "tictoc16_m.h"

using namespace omnetpp;

class Txc16 : public cSimpleModule
{
private:
    simsignal_t arrivalSignal;
protected:
    virtual TicTocMsg16 *generateMessage();
    virtual void forwardMessage(TicTocMsg16 *msg);
    virtual void initialize() override;
    virtual void handleMessage (cMessage *msg) override ;
};

    Define_Module(Txc16);

    void Txc16::initialize()
    {
        arrivalSignal =registerSignal ("arrival");

        if(getIndex() == 0)
        {
            TicTocMsg16 *msg=generateMessage();
            scheduleAt(0.0,msg);
        }

    }



void Txc16::handleMessage(cMessage *msg)
{
    TicTocMsg16 *ttmsg = check_and_cast<TicTocMsg16 *> (msg);
    if( ttmsg->getDestination() == getIndex() )
    {
        int hopcount= ttmsg->getHopCount();
        emit(arrivalSignal , hopcount);
        EV<<"Message "<<ttmsg <<" arrived after "<<hopcount<<" hops.\n";
        bubble("ARRIVED,starting new one!" );

        delete ttmsg;

        EV<<" Generating another message :";
        TicTocMsg16 *newmsg= generateMessage();
        EV<< newmsg<<endl;
        forwardMessage(newmsg);
    }
    else
    {
        forwardMessage(ttmsg);
    }
}

TicTocMsg16 *Txc16::generateMessage()
{
    int src =getIndex();
    int n= getVectorSize();
    int dest=intuniform(0,n-2);
    if(dest>=src)
    {
        dest++;
    }

    char msgname[20];
    sprintf(msgname,"tic-%d-to -%d",src,dest);

    TicTocMsg16 *msg = new TicTocMsg16(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc16::forwardMessage(TicTocMsg16 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);

    int n=gateSize("gate");
    int k=intuniform(0,n-1);

    EV<<" Forwarding message "<<msg<<" on gate["<<k<<"]\n";
    send (msg,"gate$o",k);
}

