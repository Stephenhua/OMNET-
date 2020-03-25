//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#define FSM_DEBUG
#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

/**
 * A bursty packet generator; see NED file for more info.
 */
class BurstyApp : public cSimpleModule
{
  private:
    // configuration
    int myAddress;
    std::vector<int> destAddresses;
    cPar *sleepTime;
    cPar *burstTime;
    cPar *sendIATime;
    cPar *packetLengthBytes;

    // state
    cFSM fsm;//定义一个状态机，
    enum {
        INIT = 0,
        SLEEP = FSM_Steady(1),
        ACTIVE = FSM_Steady(2),
        SEND = FSM_Transient(1),
    };

    int pkCounter;
    cMessage *startStopBurst;
    cMessage *sendMessage;
    int numSent;
    int numReceived;

    // signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

  public:
    BurstyApp();
    virtual ~BurstyApp();

  protected:
    // redefined cSimpleModule methods
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;

    // new methods
    virtual void processTimer(cMessage *msg);
    virtual void processPacket(Packet *pk);
    virtual void generatePacket();
};

Define_Module(BurstyApp);

BurstyApp::BurstyApp()//构造函数
{
    startStopBurst = sendMessage = nullptr;
}

BurstyApp::~BurstyApp()//析构函数
{
    cancelAndDelete(startStopBurst);
    cancelAndDelete(sendMessage);
}

void BurstyApp::initialize()
{
    numSent = numReceived = 0;//设置发送数和接收数
    WATCH(numSent);//时刻进行观察这两个值
    WATCH(numReceived);

    fsm.setName("fsm");//设置状态机的名称为fsm

    destAddresses = cStringTokenizer(par("destAddresses").stdstringValue().c_str()).asIntVector();
    //asIntVector 将令牌转换为整形
    //stdstringValue转化为字符串
    //。c_str()返回指向以null结尾的内容的const指针
    myAddress = par("address");
    sleepTime = &par("sleepTime");
    burstTime = &par("burstTime");
    sendIATime = &par("sendIaTime");
    packetLengthBytes = &par("packetLength");

    endToEndDelaySignal = registerSignal("endToEndDelay");
    hopCountSignal = registerSignal("hopCount");
    sourceAddressSignal = registerSignal("sourceAddress");

    pkCounter = 0;
    WATCH(pkCounter);  // always put watches in initialize(), NEVER in handleMessage()
    startStopBurst = new cMessage("startStopBurst");
    sendMessage = new cMessage("sendMessage");

    scheduleAt(0, startStopBurst);
}

void BurstyApp::handleMessage(cMessage *msg)
{
    // process the self-message or incoming packet
    if (msg->isSelfMessage())
        //isSelfMessage()如果消息由scheduleAt()发布，则返回true
        processTimer(msg);//则执行下面的程序
    else
        processPacket(check_and_cast<Packet *>(msg));//则执行下面第二个程序
}

void BurstyApp::processTimer(cMessage *msg)//接口大部分都是由指针表示
{
    simtime_t d;// 变量定义
    FSM_Switch(fsm) {
        //申明状态机，状态通过枚举类型已经声明
        case FSM_Exit(INIT):
            // transition to SLEEP state
            FSM_Goto(fsm, SLEEP);
            break;

        case FSM_Enter(SLEEP):
            // schedule end of sleep period (start of next burst)策划下一次包的冲突
            d = sleepTime->doubleValue();
            scheduleAt(simTime() + d, startStopBurst);

            // display message, restore normal icon color
            EV << "sleeping for " << d << "s\n";//表明休眠多长时间
            bubble("burst ended, sleeping");
            getDisplayString().setTagArg("i", 1, "");
            break;

        case FSM_Exit(SLEEP):
            // schedule end of this burst
            d = burstTime->doubleValue();//冲突时间
            scheduleAt(simTime() + d, startStopBurst);//开始发送冲突包

            // display message, turn icon yellow
            EV << "starting burst of duration " << d << "s\n";
            bubble("burst started");//冲突开始
            getDisplayString().setTagArg("i", 1, "yellow");//标黄色线条

            // transition to ACTIVE state:转到激活状态
            if (msg != startStopBurst)//如果消息不是开始停止冲突的这个消息
                throw cRuntimeError("invalid event in state ACTIVE");
            FSM_Goto(fsm, ACTIVE);
            break;

        case FSM_Enter(ACTIVE):
            // schedule next sending
            d = sendIATime->doubleValue();//将时间设置为整形
            EV << "next sending in " << d << "s\n";//在一个时间发送消息
            scheduleAt(simTime() + d, sendMessage);
            break;

        case FSM_Exit(ACTIVE):
            // transition to either SEND or SLEEP
            if (msg == sendMessage) {
                //如果ms是sendMessage，则执行下面的条件
                FSM_Goto(fsm, SEND);
            }
            else if (msg == startStopBurst) {
                cancelEvent(sendMessage);
                FSM_Goto(fsm, SLEEP);
            }
            else
                throw cRuntimeError("invalid event in state ACTIVE");
            break;

        case FSM_Exit(SEND): {
            // send out a packet
            generatePacket();//调用生成包函数，生成一个消息

            // return to ACTIVE
            FSM_Goto(fsm, ACTIVE);//然后转入到激活状态
            break;
        }
    }
}

void BurstyApp::generatePacket()
{
    // generate and send out a packet 形成冲突包发送出去
    int destAddress = destAddresses[intuniform(0, destAddresses.size()-1)];//由于是冲突包，所以包的目的地址是唯一的

    char pkname[40];
    sprintf(pkname, "pk-%d-to-%d-#%d", myAddress, destAddress, pkCounter++);
    EV << "generating packet " << pkname << endl;

    Packet *pk = new Packet(pkname);
    pk->setByteLength(packetLengthBytes->intValue());
    pk->setSrcAddr(myAddress);
    pk->setDestAddr(destAddress);
    send(pk, "out");
}

void BurstyApp::processPacket(Packet *pk)
{
    // update statistics and delete message
    EV << "received packet " << pk->getName() << " after " << pk->getHopCount() << "hops" << endl;
    emit(endToEndDelaySignal, simTime() - pk->getCreationTime());//从消息创建时间，到当前的仿真时间，二者之差即表示端到端延时时间
    emit(hopCountSignal, pk->getHopCount());
    emit(sourceAddressSignal, pk->getSrcAddr());
    numReceived++;
    delete pk;
}

void BurstyApp::refreshDisplay() const
{
    // update status string above icon
    char txt[64];
    sprintf(txt, "sent:%d received:%d", numSent, numReceived);
    getDisplayString().setTagArg("t", 0, txt);
}

