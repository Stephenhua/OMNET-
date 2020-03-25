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
    //������ɼ����˻�
    //int air_num;
    //int aew_num;
    //int attack_num;
    //int drone_num;


    int myAddress;// set myadress

    typedef std::map<int, int> RoutingTable;  // destaddr -> gateindex ʹ��map�����ݽṹ���洢·�ɱ�
    RoutingTable rtable; //����һ����Ϊretable��·�ɱ�

    simsignal_t dropSignal;//����ΪdropSignal��Ϣ����
    simsignal_t outputIfSignal;//����ΪoutputifSignal��Ϣ����

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Routing);

void Routing::initialize()
{
    myAddress = getParentModule()->par("address");//��ģ���еĲ���address ������ȡ������ֵ����myAddress

    dropSignal = registerSignal("drop");//��ֵID�ź�
    outputIfSignal = registerSignal("outputIf");

    //
    // Brute force approach -- every node does topology discovery on its own,
    // and finds routes to all other nodes independently, at the beginning
    // of the simulation. This could be improved: (1) central routing database,
    // (2) on-demand route calculation
    //
    cTopology *topo = new cTopology("topo");//Ϊ���˽ṹ�����µ��ڴ棬ͬʱ����һ���������͵�ָ��

    std::vector<std::string> nedTypes;//����һ����ΪNedType��������stringΪ���ݽṹ���������ͣ�
    nedTypes.push_back(getParentModule()->getNedTypeName());//�����ڵ��Ned�������ƴ洢���������������

    topo->extractByNedTypeName(nedTypes);//��ȡģ������

    cout<<"HEllo ,nm"<<endl;
    topo->extractByNedTypeName(cStringTokenizer("Node.L2Queue").asVector());//��ȡģ������
    cout<< "cTopology found " << topo->getNumNodes() << " nodes"<<endl;//�����������ܹ��ж��ٸ��ڵ�

   cTopology::Node *thisNode = topo->getNodeFor(getParentModule());//�������е�ǰ�ڵ����������еĽڵ�ָ��

    // find and store next hops
/*  for (int i = 0; i < topo->getNumNodes(); i++) {
        //����Ѱ�����˽ṹ�е��������˽ڵ�
        if (topo->getNode(i) == thisNode)
            continue;  // skip ourselves����ǵ�ǰ�ڵ㣬��ô�򽫼�������ȥ
        cout<<"HEllo ,nm_11"<<endl;
        //if(topo->getNode(i)->getModule()->getParentModule() != thisNode->getModule())
          // continue;
        cout<<"HEllo ,nm"<<endl;
        topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i));
        cout<<"HEllo ,nm____2"<<endl;
        if (thisNode->getNumPaths() == 0)//����ָ��Ŀ��ڵ�����·����
            continue;  // not connected

        cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();//CGate �ŵĶ���
        //getPath()����ָ��Ŀ��ڵ�ĵ�i�����·���е���һ�����ӡ�
        //getLocalGate�������ر������ӵ�����һ����

        int gateIndex = parentModuleGate->getIndex();//���ش���vector�е��ŵ�ID��
        int address = topo->getNode(i)->getModule()->par("address");//getModule��ʾ��ڵ����Ӧ��ģ��ĵ�ַ
        rtable[address] = gateIndex;//��gateIndex��ID�Ŵ洢���������У�
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
    delete topo;//�ͷ�֮ǰ���������ڴ�ܿռ�
}

void Routing::handleMessage(cMessage *msg)
{
    Packet *pk = check_and_cast<Packet *>(msg);//��Packet�е���Ϣ����ָ��
    int destAddr = pk->getDestAddr();//��packet�е�Ŀ�ĵ�ַ��ֵ��destAddr

    if (destAddr == myAddress) {
        EV << "local delivery of packet " << pk->getName() << endl;
        send(pk, "localOut");//����Ϣ��localOut���ͳ�ȥ
        emit(outputIfSignal, -1);  // -1: local
        return;
    }

    RoutingTable::iterator it = rtable.find(destAddr);//��·�ɱ������õ������������в���Ŀ�Ľڵ�ı��
    if (it == rtable.end()) {//
        EV << "address " << destAddr << " unreachable, discarding packet " << pk->getName() << endl;
        emit(dropSignal, (long)pk->getByteLength());//��getByteLength��Ϊ���ͳ�ȥ���ź�
        delete pk;
        return;
    }

    int outGateIndex = (*it).second;//vector����������2�У�һ����ʾ��ţ���һ����ʾĿ�Ľڵ�
    EV << "forwarding packet " << pk->getName() << " on gate index " << outGateIndex << endl;
    pk->setHopCount(pk->getHopCount()+1);//���ڲ���Ŀ�Ľڵ㣬������Ҫ���������м�1����
    emit(outputIfSignal, outGateIndex);//��outGateIndex����outputOfSignal����ȥ

    send(pk, "out", outGateIndex);//��������еĵ�outGateIndex���ŵķ��ͳ�ȥ
}

