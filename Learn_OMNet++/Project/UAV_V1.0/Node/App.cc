//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <vector>
#include <omnetpp.h>
#include "Packet_m.h"
//
#include "inet/common/ModuleAccess.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/misc/NetAnimTrace.h"
#include "inet/mobility/contract/IMobility.h"
//
using namespace omnetpp;
using namespace inet;

/**
 * Generates traffic for the network.如何形成包
 */
class App : public cSimpleModule
{
  private:
    // configuration 配置节点的基本信息
    int myAddress;  // 节点自身的信息
    std::vector<int> destAddresses; //建立一个vector向量用来惊设置目标节点
    cPar *sendIATime; // 设置指针为CPr的指针
    cPar *packetLengthBytes;

    // state 状态设置
    cMessage *generatePacket; //包形成的信息
    long pkCounter; //记数

    // signals 信号
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

  public:
    App();
    virtual ~App();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(App);

App::App()
{
    generatePacket = nullptr;//初始化
}

App::~App()
{
    cancelAndDelete(generatePacket);// 析构函数释放指针
}

void App::initialize()
{
    myAddress = par("address");//将Ned文件中节点的自身地址转换为整数型的变量
    packetLengthBytes = &par("packetLength");//将Ned文件中的存储包长度的地址由指针进行表示
    sendIATime = &par("sendIaTime");  // volatile parameter同上
    pkCounter = 0;//初始 化计数参数

    WATCH(pkCounter);//观察OKCounter的变化
    WATCH(myAddress);

    //291.4.9 用于显示节点坐标位置信息
    cModule *host = getContainingNode(this);
    inet::IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
    Coord cor= mobility->getCurrentPosition();
    EV<<"my coords are "<<cor.x<<" "<<cor.y<<endl;
    //

    const char *destAddressesPar = par("destAddresses");//将目的地址设置所需要的指针设置为常量
    cStringTokenizer tokenizer(destAddressesPar);//这一段是什么意思？字符串转化吗
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)
        destAddresses.push_back(atoi(token));//如果不是空指针，则将数值压入到里面

    if (destAddresses.size() == 0)
        throw cRuntimeError("At least one address must be specified in the destAddresses parameter!");

    generatePacket = new cMessage("nextPacket");
    scheduleAt(sendIATime->doubleValue(), generatePacket);

    endToEndDelaySignal = registerSignal("endToEndDelay");//将端到端延迟的信号ID赋值给信号ID
    hopCountSignal = registerSignal("hopCount");
    sourceAddressSignal = registerSignal("sourceAddress");
}

void App::handleMessage(cMessage *msg)//用于控制消息的发送和接收
{
    //2019.4.9 用于显示节点坐标位置信息
    cModule *host = getContainingNode(this);
    inet::IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
    Coord cor= mobility->getCurrentPosition();
    EV<<"my coords are "<<cor.x<<" "<<cor.y<<endl;
    //
    if (msg == generatePacket) {
        // Sending packet
        //if msg is equal to genergateMessage ,continue;
        int destAddress = destAddresses[intuniform(0, destAddresses.size()-1)];//destAddress is random select

        char pkname[40];//display the string ,set the
        sprintf(pkname, "pk-%d-to-%d-#%ld", myAddress, destAddress, pkCounter++);//print the route in table
        EV << "generating packet " << pkname << endl;

        Packet *pk = new Packet(pkname);//定义包中的属性
        pk->setByteLength(packetLengthBytes->intValue());//将packetLengthBytes进行整数整形，并将其设置为pk的字节长度
        pk->setKind(intuniform(0, 7));//设置消息的类型
        pk->setSrcAddr(myAddress);//设置源地址，将myAdress直接传递给pk
        pk->setDestAddr(destAddress);//设置目的地址
        send(pk, "out");//将消息从out口发出去


        scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);//generatePacket 相当于全局变量
        if (hasGUI())
            getParentModule()->bubble("Generating packet...");
    }
    else {
        // Handle incoming packet
        //控制接收到的消息
        Packet *pk = check_and_cast<Packet *>(msg);
        EV << "received packet " << pk->getName() << " after " << pk->getHopCount() << "hops" << endl;
        emit(endToEndDelaySignal, simTime() - pk->getCreationTime());
        emit(hopCountSignal, pk->getHopCount());
        emit(sourceAddressSignal, pk->getSrcAddr());
        delete pk;

        if (hasGUI())
            getParentModule()->bubble("Arrived!");
    }
}

