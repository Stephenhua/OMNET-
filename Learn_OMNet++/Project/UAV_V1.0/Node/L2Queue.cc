//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <map>
#include "Packet_m.h"
# include <iostream>
using namespace std;
using namespace omnetpp;

/**
 * Point-to-point interface module. While one frame is transmitted,
 * additional frames get queued up; see NED file for more info.
 */
class L2Queue : public cSimpleModule
{
  private:
    long frameCapacity;

    cQueue queue;
    cMessage *endTransmissionEvent;//������¼�
    bool isBusy;//�ж϶����Ƿ�æ

    simsignal_t qlenSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;
    simsignal_t dropSignal;
    simsignal_t txBytesSignal;
    simsignal_t rxBytesSignal;

  public:
    int address;
    map<int,cModule *> allUAV;//֮������޸�
    L2Queue();
    virtual ~L2Queue();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    virtual void startTransmitting(cMessage *msg);
};

Define_Module(L2Queue);

L2Queue::L2Queue()
{
    endTransmissionEvent = nullptr;//��Ϣ�¼���ָ��
}

L2Queue::~L2Queue()
{
    cancelAndDelete(endTransmissionEvent);
}

void L2Queue::initialize()
{
    address = getParentModule()->par("address").intValue();
    int myindex = this->getIndex();
    address = myindex;

    queue.setName("queue");//���ö��е�����Ϊqueue
    endTransmissionEvent = new cMessage("endTxEvent");//ΪnewTxEvent����һ���µ��ڴ�

    if (par("useCutThroughSwitching"))
        gate("line$i")->setDeliverOnReceptionStart(true);

    frameCapacity = par("frameCapacity");

    qlenSignal = registerSignal("qlen");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    dropSignal = registerSignal("drop");
    txBytesSignal = registerSignal("txBytes");
    rxBytesSignal = registerSignal("rxBytes");


    cModule *topview = this->getModuleByPath("<root>");//�ҵ����ڵ�ģ�ͼ��ڵ�
        for (cModule::SubmoduleIterator it(topview); !it.end(); ++it) {
            cModule *parentmodule = *it;

            // ���������˻��洢��allUAV
            allUAV[(int)parentmodule->par("address")] = (cModule *)parentmodule;
        }
    emit(qlenSignal, queue.getLength());    //��qlenSignal����Ϊqueue.getLength
    emit(busySignal, false); // ��������Singnal��Ϣ����Ϊfasle


    isBusy = false;
}
/*
void L2Queue::startTransmitting(cMessage *msg)//��ʼ�����״̬
{
    EV << "Starting transmission of " << msg << endl;
    isBusy = true;
    int64_t numBytes = check_and_cast<cPacket *>(msg)->getByteLength();
    //send(msg, "line$o");//��line������ڽ���Ϣ����
    Packet *pk = (Packet *)msg;
    cout<<"H1"<<endl;

    simtime_t txFinishTime = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    if((txFinishTime == -1) || (txFinishTime < simTime())){
    if((txFinishTime < simTime())){
        // send(toUAVc,"gate$o",rtable[toUAVc->getUavAddr(i)]);
        sendDirect(pk,0.1,0.2,allUAV[(int)pk->getDestAddr()],"port$i",address);
      }
     else{
         sendDirect(pk,txFinishTime-simTime(),0.0001,allUAV[(int)pk->getDestAddr()],"port$i",address);
        }

    emit(txBytesSignal, (long)numBytes);//guiding txByteSignal��Ϣ�ĳ���
    cout<<"H2"<<endl;
    // Schedule an event for the time when last bit will leave the gate.
   simtime_t endTransmission = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
   scheduleAt(endTransmission, endTransmissionEvent);//��endTransmissionʱ�̴�����Ϣ������Ϣ�ǳ�ʼ��ʱ��Ĵ�Message�� msgָ����ָ���
}

*/
void L2Queue::startTransmitting(cMessage *msg)
{
    EV << "Starting transmission of " << msg << endl;
    isBusy = true;
    int64_t numBytes = check_and_cast<cPacket *>(msg)->getByteLength();
    emit(txBytesSignal, (long)numBytes);
    //send(msg, "line$o");
    Packet  *pk=(Packet *) msg;
    cout<<"hell0"<<endl;
    //cChannel *txChannel = gate("line$o",allUAV[(int)pk->getDestAddr()])->getTransmissionChannel();
   // simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
    simtime_t txFinishTime = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    if((txFinishTime == -1) || (txFinishTime < simTime())){
             // send(toUAVc,"gate$o",rtable[toUAVc->getUavAddr(i)]);
        cout<<"Hello_11"<<endl;

           sendDirect(pk,0,0,allUAV[(int)pk->getDestAddr()],"port$i",address);
           cout<<"Hello_1"<<endl;
    }
    else{
        cout<<"Hello_2"<<endl;
            sendDirect(pk,txFinishTime-simTime(),0.5,allUAV[(int)pk->getDestAddr()],"port$i",address);
     }


    // Schedule an event for the time when last bit will leave the gate.
    cout<<"Hello2"<<endl;
   simtime_t endTransmission = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    scheduleAt(simTime(), endTransmissionEvent);
}

void L2Queue::handleMessage(cMessage *msg)
{
    if (msg == endTransmissionEvent) {
        // Transmission finished, we can start next one.
        //��ǰһ��������Ϣ������ɺ��������һ����Ϣ�Ĵ���
        EV << "Transmission finished.\n";
        isBusy = false;
        if (queue.isEmpty()) {
            emit(busySignal, false);
        }
        else {
            msg = (cMessage *)queue.pop();
            emit(queueingTimeSignal, simTime() - msg->getTimestamp());//�Ŷ�ʱ��Ϊ��ǰ����ʱ��-msg��ʱ���
            emit(qlenSignal, queue.getLength());//���г���
            startTransmitting(msg);
        }
    }
    else if (msg->arrivedOn("line$i")) {
        // pass up ����Ϣ������˵���ʱ����Ӧ����β���
        cout<<"receive"<<endl;
        emit(rxBytesSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());//ȷ�����յ�����Ϣ�ĳ���
        send(msg, "out");

    }
    else {  // arrived on gate "in"
        if (endTransmissionEvent->isScheduled())
        //�жϵ�ǰ�Ƿ�����ϢҪ����
        {
            // We are currently busy, so just queue up the packet.
            if (frameCapacity && queue.getLength() >= frameCapacity) {
                EV << "Received " << msg << " but transmitter busy and queue full: discarding\n";
                emit(dropSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
                delete msg;
            }
            else {
                EV << "Received " << msg << " but transmitter busy: queueing up\n";
                msg->setTimestamp();
                queue.insert(msg);
                emit(qlenSignal, queue.getLength());
            }
        }
        else {
            // We are idle, so we can start transmitting right away.
            EV << "Received " << msg << endl;
            emit(queueingTimeSignal, SIMTIME_ZERO);//queueingTimeSignal �������ж��Ŷ���Ҫ�೤ʱ����Ϣ���ܷ���
            startTransmitting(msg);//������Ϣ
            emit(busySignal, true);
        }
    }
}

void L2Queue::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 0, isBusy ? "transmitting" : "idle");
    getDisplayString().setTagArg("i", 1, isBusy ? (queue.getLength() >= 3 ? "red" : "yellow") : "");
}

