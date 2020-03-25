# include <omnetpp.h>
# include <string.h>
# include <stdio.h>

using namespace omnetpp;

class Txc14:public cSimpleModule
{
    private:
    long numSent;
    long numRevecive;
    protected:
    virtual TicToc14 *generateMessage();
    virtual initialize() override;
    virtual handleMessage(cMessage *msg) override;
    virtual forwardMessage(cMessage *msg);
    virtual refreshDisplay() const override;

}

Define_Module(Txc14);

void Txc14::initialize()
{
    numSent=0;
    numRevecive=0;
    WATCH(numSent);
    WATCH(numReceive);
    if(getIndex()==0)
    {
        TicToc14 *msg=generateMessage();
        numSent++;
        EV<<"intial message is "<<msg<<"\n";
        scheduleAt(0.0,msg);
    }
}

void Txc14::handleMessage(cMessage *msg)
{
    TicToc14 *ttmsg =check_and_cast<TicTocMsg14>(msg);

    if(ttmsg->getDestination()==getIndex())
    {
        int hopcount=ttmsg->getHopCount();
        EV<<"Message "<<ttmsg<<"arrived after "<<hopcount<<"hops\n";
        
        numRevecive++;
        delete ttmsg;
        bubble("Message arrived");

        EV<"Generating new message\n";

        TicToc14 *newmsg=generateMessage();

        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        forwardMessage(ttmsg);
    }
    
}

TicToc14 *Txc14::generateMessage()
{
    int src=getSource();
    
    int n=getVertorSize();
    int dest=intuniform(0,n-2);
    if(dest>src)
    {
        dest++;
    }
    char msgname[30];
    sprintf(msgname," %d to %d ",src,dest);

    TicToc14 *msg=new TicToc14(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;

}


void forwardMessage(cMessage *msg)
{
    msg->setHopCount(msg->getHopCount()+1);

    int n=gateSize("gate");
    int k=intuniform(0,n-2);

    EV<<"Forwading message "<<msg<<"on gate ["<<k<<"]"<<"\n";
    send(msg,"gate$o",k);
}

void Txc14::refreshDisplay() const 
{
    char buf[40];
    sprintf(buf,"%ld sent : %ld",numRevecive,numSent);
    getDisplayString().setTagArg("t",0,buf);
}