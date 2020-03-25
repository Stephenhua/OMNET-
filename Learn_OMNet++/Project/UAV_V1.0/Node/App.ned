//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package Node;

//
// Application model to generate traffic for the network.
//形成信息的子模块
//
simple App like IApp
{
    parameters:
        int address;  // local node address 本节点的地址
        string destAddresses;  // destination addresses 目的节点的地址
        volatile double sendIaTime @unit(s) = default(exponential(1s)); // time between generating packets 发送包与包之间的时间间隔； volatile 可以保证对特殊地址的稳定访问
        volatile int packetLength @unit(byte);  // length of one message (fixed! no "volatile" modifier) 包的长度
        @display("i=block/browser");
        @signal[endToEndDelay](type="simtime_t");//端到端延时信号
        @signal[hopCount](type="long");//跳数的信号
        @signal[sourceAddress](type="long");//目的地址信号
        @statistic[endToEndDelay](title="end-to-end delay of arrived packets";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[hopCount](title="hop count of arrived packets";interpolationmode=none;record=vector?,mean,max);
        @statistic[sourceAddress](title="source address of arrived packets";interpolationmode=none;record=vector?);
    gates:
        input in;
        output out;
}


