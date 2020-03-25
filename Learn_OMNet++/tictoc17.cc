# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
# include "tictoc17_m.h"

using namespace omnetpp;
class Txc17 public:cSimpleModule
{
    private:
    simsignal_t arrivalSignal;
    protected:
    virtual TicTocMsg17 *generateMessage();
    virtual void forwardMessage(TicTocMsg17 *msg );
    virtual  void initialize()override;
    virtual void handleMessage(cMessage *msg ) override ;

};

Define_Module(Txc17)

void Txc17::initialize()
{
    arrivalSignal=registerSignal("arrival");
    if(getIndex()==0)
    {
        TicTocMsg17 *msg=generateMessage();
        scheduleAt(0.0,msg);
   }
}

void Txc17::handleMessage(cMessage *msg)
{
    TicTocMsg17 *ttmsg=check_and_cast<TicTocMsg17 *> (msg);
    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount = getHopCount()+1;
        emit(arrivalSignal , hopcount);
        if(hasGUI())
        {
            char label[40];
            sprintf(label,"last hopcount = %d ",hopcount);
            cCanvas *canvas=getParentModule()->getCanvas();
            cTextFigure *textFigure =check_and_cast<cTextFigure *>(canvas->getFigure("lasthopcount"));
            TextFigure->getText(label);
        }

        EV<<"Message "<<ttmsg<<"arrived after "<<hopcount<<"hops \n";
        bubble("ARRIVED, starting new one!");
        delete ttmsg;

        TicTicMsg17 *newmsg=generateMessage();
        EV<<newmsg<<endl;
        forwardMessage(newmsg);
    }
    else
    {
        forwardMessage(ttmsg);
    }
    
}

TicTocMSg17 *Txc17::generateMessage()
{
    int src=getSource();
    int k=getVecotrSize();
    int dest=intunifrom(0,k-2);
    if(dest>=src)
    {
        dest++;
    }
    
    charname[50];
    sprintf(charname,"%d to %d.",src,dest);

    TicTocMsg17 *msg=new TicTocMsg17(msgname);
    msg->setDestination(dest);
    msg->setSource(sur);
    return msg;

}

void Txc17::forwardMessaeg(TicTocMsg17 *msg)
{
    msg->setHopCount(msg->getHopCount()+1);
    int n=gateSize("gate");
    int k=intuniform(0,n-1);

    EV<<"Forwarding message "<<msg<<"on gate ["<<k<<"]\n";
    send(msg,"gate$o",k);
}