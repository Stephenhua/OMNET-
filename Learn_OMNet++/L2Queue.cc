# include <stdio.h>
# include <string.h>
# include <omnetpp.h>

using namespace omnetpp;

class L2Queue : public cSimpleModule
{
    private:
        long frameCapacity;
        cQueue queue;
        cMessage *endTransmissionEvent;
        bool isBusy;

        simsignal_t qlenSingal;
        simsignal_t busySignal;
        simsignal_t queueingTimeSignal;
        simsignal_t  dropSignal;
        simsignal_t txBytesSignal;
        simsignal_t rxBytesSignal;
    
    public:
        L2Queue();
        virtual ~L2Queue();

    protected:
        virtual void initialize() override ;
        virtual void handleMessage (cMessage *msg) override ;
        virtual void refreshDisplay()const override ;
        virtual void startTransmitting(cMessage *msg);
};

Define_MOdule (L2Queue);

L2Queue::L2Queue()
{
    endTransmissionEvent =nullptr;
}

L2Queue::~L2Queue()
{
    cancelAndDelete(endTransmissionEvent);
}

void L2Queue::initialize()
{
    queue.setName("queue");
    endTransmissionEvent =new cMessage("endTxEvent");
    if(par("useCutThroughSwitching"))
    {
        gate("line$i")->setDeliverInReceptionStart(true);
    }

    frameCapacity=par("frameCapacity");

    qlenSingal= registerSignal("qlen");
    busySignal =registerSignal("busy");
    queueingTimeSignal =registerSignal("queueingTime");
    dorpSignal= registerSignal("drop");
    rxBytesSignal =registerSignal("rxBytes");
    txBytesSignal= registerSignal("txBytes");

    emit(qlenSingal,queue.getLength());
    emit(busySignal,false);
    isBusy= false;
}

void L2Queue::startTransmitting(cMessage *msg)
{
    EV<<"Starting transmission of "<< msg <<endl;
    isBusy =true;
    int64_t numBytes= check_and_cast<cPacket*>(msg);

    send(msg,"line$o");
    emit(txBytesSignal,(long)numBytes);

    simtime_t endTransmission= gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    scheduleAt(endTransmission,endTransmissionEvent);
}

void L2Queue::handleMessage(cMessage *msg)
{
    if(msg==endTransmissionEvent)
    {
        EV<<"Transmission finished .\n";
        isBusy= fasle;
        if(queue.isEmpty())
        {
            emit(busySignal,false);
        }
        else
        {
            msg=(cMessage *) queue.pop();
            emit(queueingTimeSignal, simTime()- msg->getTimestamp());
            emit(qlenSignal,queue.getlength());
            startTransmitting(msg);
        }
        
    }
    else if(msg->arrivedOn("line$i"))
    {
        emit(rxBytesSignal,(long)check_and_cast<cPacket *>(msg));
        send(msg,"out");
    }
    else
    {
        if(endTransmissionEvent->isScheduled())
        {
            if(frameCapacity&&queue.getLength()>=frameCapacity)
            {
                EV<<"Received "<<msg<<"but transmitter busy and queue full ：discarding \n";
                emit(dorpSignal,(long )check_and_cast<cPacket *>(msg)->getLength());
                delete msg;
            }
            else
            {
                EV<<"Recevied "<< msg<< " but transmitter busy :queueing\n";
                msg->setTimestamp();
                queue.insert(msg);
                emit(qlenSingal,queue.getLength());
            }
            
        }
        else
        {
            EV<<"Recevived "<<msg<<endl;
            emit(queueingTimeSignal,SIMTIME_ZERO);
            startTransmitting(msg);
            emit(busySignal,true);
        }
        
    }
    
}

void L2Queue::refreshDisplay()const 
{
    getDisplayString().setTagArg("t",0,isBusy?"transmitting":"idle");
    getDispalyString().setTagArg("i",1,isBusy？(getLength()>=3?"red":"yellow")："");
}