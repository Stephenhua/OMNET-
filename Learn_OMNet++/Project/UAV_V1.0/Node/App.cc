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
 * Generates traffic for the network.����γɰ�
 */
class App : public cSimpleModule
{
  private:
    // configuration ���ýڵ�Ļ�����Ϣ
    int myAddress;  // �ڵ��������Ϣ
    std::vector<int> destAddresses; //����һ��vector��������������Ŀ��ڵ�
    cPar *sendIATime; // ����ָ��ΪCPr��ָ��
    cPar *packetLengthBytes;

    // state ״̬����
    cMessage *generatePacket; //���γɵ���Ϣ
    long pkCounter; //����

    // signals �ź�
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
    generatePacket = nullptr;//��ʼ��
}

App::~App()
{
    cancelAndDelete(generatePacket);// ���������ͷ�ָ��
}

void App::initialize()
{
    myAddress = par("address");//��Ned�ļ��нڵ�������ַת��Ϊ�����͵ı���
    packetLengthBytes = &par("packetLength");//��Ned�ļ��еĴ洢�����ȵĵ�ַ��ָ����б�ʾ
    sendIATime = &par("sendIaTime");  // volatile parameterͬ��
    pkCounter = 0;//��ʼ ����������

    WATCH(pkCounter);//�۲�OKCounter�ı仯
    WATCH(myAddress);

    //291.4.9 ������ʾ�ڵ�����λ����Ϣ
    cModule *host = getContainingNode(this);
    inet::IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
    Coord cor= mobility->getCurrentPosition();
    EV<<"my coords are "<<cor.x<<" "<<cor.y<<endl;
    //

    const char *destAddressesPar = par("destAddresses");//��Ŀ�ĵ�ַ��������Ҫ��ָ������Ϊ����
    cStringTokenizer tokenizer(destAddressesPar);//��һ����ʲô��˼���ַ���ת����
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)
        destAddresses.push_back(atoi(token));//������ǿ�ָ�룬����ֵѹ�뵽����

    if (destAddresses.size() == 0)
        throw cRuntimeError("At least one address must be specified in the destAddresses parameter!");

    generatePacket = new cMessage("nextPacket");
    scheduleAt(sendIATime->doubleValue(), generatePacket);

    endToEndDelaySignal = registerSignal("endToEndDelay");//���˵����ӳٵ��ź�ID��ֵ���ź�ID
    hopCountSignal = registerSignal("hopCount");
    sourceAddressSignal = registerSignal("sourceAddress");
}

void App::handleMessage(cMessage *msg)//���ڿ�����Ϣ�ķ��ͺͽ���
{
    //2019.4.9 ������ʾ�ڵ�����λ����Ϣ
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

        Packet *pk = new Packet(pkname);//������е�����
        pk->setByteLength(packetLengthBytes->intValue());//��packetLengthBytes�����������Σ�����������Ϊpk���ֽڳ���
        pk->setKind(intuniform(0, 7));//������Ϣ������
        pk->setSrcAddr(myAddress);//����Դ��ַ����myAdressֱ�Ӵ��ݸ�pk
        pk->setDestAddr(destAddress);//����Ŀ�ĵ�ַ
        send(pk, "out");//����Ϣ��out�ڷ���ȥ


        scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);//generatePacket �൱��ȫ�ֱ���
        if (hasGUI())
            getParentModule()->bubble("Generating packet...");
    }
    else {
        // Handle incoming packet
        //���ƽ��յ�����Ϣ
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

