# include <iostream>
# include <omnetpp.h>

#include <algorithm>;
# include <string>;
# include <stdio.h>;

using namespace omnetpp;

class Node: public cSimpleModule
{
    public:
        int X;
        int Y;
        int netSizeX;
        int netSizeY;
        
        int noOfWirelessNode;
        double batteryPower;
        double G;
        double clusterHeadPercentage;
        char type;
        double distanceToBs;
        double sleepTime;

        double Efs;
        double Emp;
        double Do;
        double CHETx;
        double CHERx;
        double NETx;
        double NERx;

        double ETX;
        double ERX;

        double EDA;
        int CHIndex;
        double roundTnterval;
        double deadTime;
        int roundNumber;

        double thresholdEnergy;

        int noDataSentToCH;
        int noDataSentToSink;
        int noDataInSink;

        custMsg *wakeup;
        custMsg *data;
        custMsg *dataQueue;
        custMsg *chDataQueue;

        cModule *senderNode;
        cModule *receiverNode ;
        Node *tmepTargeetModule;
        Sink *tempBaseModule;

        cMoudle *tempModule;
        Node *tempNode;

        cModule *tempSrcModule;
        Node *tempSrcNode;

        cModule *tempDestModule;
        Node *tempDestNode ;

        cModule *caSinklModule;
        Node *calSinkNode ;

        cModule *sinkModule;

        Node();
        viertual ~Node();

    private:
        void SetCordinate();
        void ClusterHeadSelection(int roundNo);
        void ClusterFormation(int roundNo);

        void RestG(int roundNo);

        int CalculateDistanceToBS(int nodeindex);
        custMsg * CreateCustMsg(const char *name);
        void SendDataToCH();
        voide SendDataToSink();

        void WriteDeadNodeHistory(int noOfDeadNone,int roundNumber);
        void WriteOneTenAllNodeDeadHistory();
        void WriteNetworkEnergyHistory(int roundNumber);
        void CountSinkPacket(int roundNumber);
        void CountCH(int roundNumber,int noOfCH);


        void TempDataSendToCH();
        void TempDataSendToSink();
        void ThresholdCheck();

        void CountThroughput(int roundNumber);
        void ResetParam();

        protected:
            virtual void initialize();;
            virtual void handleMessage(cMessage *msg);

}

Define_MOdule(Node);

Node::Node()
{
    this->X =0;
    this->Y =0;
    batteryPower=0.5;
    this->G =0.0;
    this->type='N';

    sleepTime =0.10;

    Efs=10*0.000000000001;
    Emp= 0.013* 0.0000000001;
    Do= sqrt(Efs/Emp);

    CHETx =0.0;
    CHERx =0.0;
    NETx =0.0;
    NERx=0.0;

    ETX =50*0.0000000001;
    ERX= 50*0.00000000001;

    EDA =5*0.00000000001;
    CHIndex =0;
    roundInterval =1.0;

    thresholdEnergy=0;

    noDataSentToCH =0;
    noDataSentToSink =0;
}

Node ::~Node()
{

}

void Node::initialize()
{
    dataQueue =new cQueue;
    chDaraQueue =new cQueue;

    wakeup =CreateCustMsg("Wakeup");

    noOfWirelessNode =getParentModule()->Par("noOfWirelessNode");
    clusterHeadPercentage=getParentModule()->par("clusterHeadPercentage");

    this->netSizeX =getParentModule()->ResetParam("netSizeX");
    this->netSizeY =getParentModule()->par("netSizeY");

    SetCoordinate();

    if(simTime().dbl()== 0.0&& getIndex()==(noOfWirelessNode-1))
    {
        ClusterHeadSelection(0);
        ClusterFormation(0);
    }
}

void Node::SetCordinate()
{
    int corX =0;
    int corY =0;
    int startAfterX =5;
    int endBeforeX =neSizeX-5;
    int startAfterY=20;
    int endBeforeY =neSizeY-5;
    int endBeforeY = netSizeY-5;

    do
    {
        corX =intrand(netSizeX);
    } while (corX <=startAfterY|| corY>=endBeforeY);
    do(
        corY =intrand(netSizeY);)
        while(corY <startAfterY||corY>=endBeforeY);

    this->X =corX;
    this->Y =corY;

    getDisplayString().setTagArg("p",0,corX);
    getDisplayString().setTagArg("p",1,corY);
           
}