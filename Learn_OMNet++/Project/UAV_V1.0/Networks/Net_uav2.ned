//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package Networks;

import Node.NodeDrone;
import Node.NodeAew;

import Node.NodeAttack;
import ned.DatarateChannel;

//
// A ten-node network topology
//
network Net_uav2
{
    types:
        channel C extends DatarateChannel
        {
            delay = default(uniform(0.1ms, 1ms));
            datarate = default(1Mbps);
             @display("ls=,,da");
        }
    submodules:
        drone[10]: NodeDrone {//无人机节点
            address = index+1;
        }
        aew[1]:NodeAew{//预警机节点
            address=index+11;
             gates:
                port[19];
        }
      
        attack[4]:NodeAttack{//攻击机节点
        	address =index+15;
        }
            
 connections allowunconnected:

         // for i=0..2,for j=i..2 {
 	  //     aew[0].port[j]  <--> C <--> tans[i].port[j];
        //  aew[1].port[j]  <--> C <--> tans[i].port[j];
        //  }
          
          for i=0..3 ,for j=i..3 {
    	      aew[0].port[j+3]  <--> C <--> attack[i].port[j];
			   aew[1].port[j+3]  <--> C <--> attack[i].port[j];
          }
        
        
          for i=0..9,for j=i..9
         {
              aew[0].port[j+7]  <--> C <--> drone[i].port[j];
              aew[1].port[j+7]  <--> C <--> drone[i].port[j];
         }
        
           for i=0..9,for j=i..9 {
           drone[i].port[j+10] <--> C <--> drone [j].port[i+10];
         }
    // for i=0..n-2, for j=i+1..n-1 {
    //      node[i].port[j] <--> {  delay = 0.1ms; datarate = 1Gbps; } <--> node[j].port[i] if uniform(0,1)<0.15;
    //     }
        
        
}

