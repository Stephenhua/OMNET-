/*
 * tic7.cc
 *
 *  Created on: 2018Äê10ÔÂ22ÈÕ
 *      Author: Jason
 */
# include <stdio.h>
# include <string.h>
# include <omnetpp.h>
 using namespace omnetpp;

 class Txc7 : public cSimpleModule
 {
 private:
      cMessage *event;
      cMessage *tictocMsg;
 public:
      Txc7();
      virtual ~Txc7();
 protected:
      virtual void initialize() override;
      virtual void handleMessage(cMessage *msg) override;
 };

 Define_Module(Txc7);
 Txc7::Txc7()
 {
     event=tictocMsg=nullptr;
 }

 Txc7::~Txc7()
 {
     cancelAndDelete(event);
     delete tictocMsg;
 }

 void Txc7::initialize()
 {
     event =new cMessage("event");
     tictocMsg=nullptr;

     if(strcmp("tic",getName()) == 0)
     {
         EV<<"Scheduling first send to t=5.0s\n";
         scheduleAt(5.0,event);
         tictocMsg= new cMessage("tictocMSg");
     }
 }

 void Txc7::handleMessage(cMessage *msg)
 {
     if(msg==event)
     {
         EV<<"Wait period is over , sending back message\n";
          send(tictocMsg,"out");
         tictocMsg=nullptr;

     }
     else
     {
         if(uniform(0,1)<0.1)
         {
             EV<<"\"Losing\"message\n";
             delete msg;
         }
         else
         {
         simtime_t delay=par("delayTime");
         EV<<"Message arrived ,starting to wait"<<delay<<"secs..\n";
         tictocMsg = msg;
         scheduleAt(simTime()+delay,event);
         }
     }
 }
