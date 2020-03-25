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
// A "Node" consists of a Routing plus an App module which represents the
// aggregate traffic of the subnets connected to that router.
//

import inet.common.lifecycle.NodeStatus;
import inet.mobility.contract.IMobility;
import inet.node.inet.INetworkNode;
import inet.power.contract.IEnergyGenerator;
import inet.power.contract.IEnergyManagement;
import inet.power.contract.IEnergyStorage;
import ned.DatarateChannel;
 channel F extends DatarateChannel
        {
            //delay = default(uniform(0.0001ns, 0.001ns));
            delay = default(uniform(0.1s, 1s));
            datarate = default(10Mbps);
             @display("ls=,,da");
        }


module NodeDrone
{
    parameters:
        //无人机进行配置
       // int aew_num;
        //int attack_num;
         //int drone_num;
        //上述作为可选或者可不选
        //设定节点的位置坐标

        int address;
        string appType;
        @networkNode();
        @display("i=device/drone_1,bgb=288,273");
        @statistic[coordX](title="coord X"; source="xCoord(mobilityPos(mobilityStateChanged))"; record=vector; interpolationmode=linear);
        @statistic[coordY](title="coord Y"; source="yCoord(mobilityPos(mobilityStateChanged))"; record=vector; interpolationmode=linear);
        @statistic[coordZ](title="coord Z"; source="zCoord(mobilityPos(mobilityStateChanged))"; record=vector; interpolationmode=linear);
    gates:
        // inout  port[19] ;
       inout  port[10] @loose;
    submodules:
        app: <appType> like IApp {
            parameters:
                address = address;
                @display("p=140,60");
        }
        routing: Routing {
            parameters:
                @display("p=140,130");
            gates:
                in[sizeof(port)];
                out[sizeof(port)];
        }
        queue[sizeof(port)]: L2Queue {
            parameters:
                @display("p=163.52,189.8,row");
        }
        mobility: <default("")> like IMobility if typename != "" {
            parameters:
                @display("p=17,82;is=s");
        }
    connections:
        routing.localOut --> app.in;
        routing.localIn <-- app.out;
        for i=0..sizeof(port)-1 {
            routing.out[i] --> queue[i].in;
            routing.in[i] <-- queue[i].out;
            queue[i].line <--> F <--> port[i];
        }


}


