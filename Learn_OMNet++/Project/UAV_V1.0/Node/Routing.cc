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

#include <map>
#include <omnetpp.h>
#include "Packet_m.h"
#include <iostream>
using namespace std;
using namespace omnetpp;

/**
 * Demonstrates static routing, utilizing the cTopology class.
 */
class Routing : public cSimpleModule
{
  private:
    //添加若干架无人机
    //int air_num;
    //int aew_num;
    //int attack_num;
    //int drone_num;


    int myAddress;// set myadress

    typedef std::map<int, int> RoutingTable;  // destaddr -> gateindex 使用map的数据结构来存储路由表
    RoutingTable rtable; //定义一个名为retable的路由表

    simsignal_t dropSignal;//设置为dropSignal消息类型
    simsignal_t outputIfSignal;//设置为outputifSignal消息类型

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Routing);

void Routing::initialize()
{
    myAddress = getParentModule()->par("address");//父模块中的参数address 进行提取，将其值付给myAddress

    dropSignal = registerSignal("drop");//赋值ID信号
    outputIfSignal = registerSignal("outputIf");

    //
    // Brute force approach -- every node does topology discovery on its own,
    // and finds routes to all other nodes independently, at the beginning
    // of the simulation. This could be improved: (1) central routing database,
    // (2) on-demand route calculation
    //
    cTopology *topo = new cTopology("topo");//为拓扑结构分配新的内存，同时创建一个拓扑类型的指针

    std::vector<std::string> nedTypes;//设置一个名为NedType的向量表，string为数据结构的数组类型，
    nedTypes.push_back(getParentModule()->getNedTypeName());//将父节点的Ned类型名称存储到的这个向量表中

    topo->extractByNedTypeName(nedTypes);//提取模型拓扑

    cout<<"HEllo ,nm"<<endl;
    topo->extractByNedTypeName(cStringTokenizer("Node.L2Queue").asVector());//提取模型拓扑
    cout<< "cTopology found " << topo->getNumNodes() << " nodes"<<endl;//发现拓扑中总共有多少个节点

   cTopology::Node *thisNode = topo->getNodeFor(getParentModule());//将拓扑中当前节点用拓扑类中的节点指出

    // find and store next hops
/*  for (int i = 0; i < topo->getNumNodes(); i++) {
        //用于寻找拓扑结构中的其他拓扑节点
        if (topo->getNode(i) == thisNode)
            continue;  // skip ourselves如果是当前节点，那么则将继续跳出去
        cout<<"HEllo ,nm_11"<<endl;
        //if(topo->getNode(i)->getModule()->getParentModule() != thisNode->getModule())
          // continue;
        cout<<"HEllo ,nm"<<endl;
        topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i));
        cout<<"HEllo ,nm____2"<<endl;
        if (thisNode->getNumPaths() == 0)//返回指向目标节点的最短路径数
            continue;  // not connected

        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();//CGate 门的对象
        //getPath()返回指向目标节点的第i个最短路径中的下一个链接。
        //getLocalGate（）返回本地连接的最后的一个门

        int gateIndex = parentModuleGate->getIndex();//返回存在vector中的门的ID号
        int address = topo->getNode(i)->getModule()->par("address");//getModule表示与节点相对应的模块的地址
        rtable[address] = gateIndex;//将gateIndex的ID号存储到向量表中；
        EV << "  towards address " << address << " gateIndex is " << gateIndex << endl;
    }
    aew_num=getParentModule()->par("aew_num");
    attack_num=getParentModule()->par("attack_num");
    drone_num= getParentModule()->par("drone_num");
    air_num=aew_num+attack_num+drone_num;
*/
      for(int i=0;i<10;i++)
    {
        rtable[i]=i;
    }
    cout<<"endle"<<endl;
    delete topo;//释放之前所创建的内存管空间
}

void Routing::handleMessage(cMessage *msg)
{
    Packet *pk = check_and_cast<Packet *>(msg);//将Packet中的消息赋予指针
    int destAddr = pk->getDestAddr();//将packet中的目的地址赋值给destAddr

    if (destAddr == myAddress) {
        EV << "local delivery of packet " << pk->getName() << endl;
        send(pk, "localOut");//将消息从localOut发送出去
        emit(outputIfSignal, -1);  // -1: local
        return;
    }

    RoutingTable::iterator it = rtable.find(destAddr);//在路由表中设置迭代器，在其中查找目的节点的标号
    if (it == rtable.end()) {//
        EV << "address " << destAddr << " unreachable, discarding packet " << pk->getName() << endl;
        emit(dropSignal, (long)pk->getByteLength());//将getByteLength作为发送出去的信号
        delete pk;
        return;
    }

    int outGateIndex = (*it).second;//vector向量表中有2列，一个表示标号，另一个表示目的节点
    EV << "forwarding packet " << pk->getName() << " on gate index " << outGateIndex << endl;
    pk->setHopCount(pk->getHopCount()+1);//由于不是目的节点，所以需要将跳数进行加1处理
    emit(outputIfSignal, outGateIndex);//将outGateIndex，从outputOfSignal发出去

    send(pk, "out", outGateIndex);//从输出口中的第outGateIndex的门的发送出去
}

