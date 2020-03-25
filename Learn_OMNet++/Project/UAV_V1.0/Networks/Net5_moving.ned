//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package Networks;

import Node.Node;
import ned.DatarateChannel;

import inet.visualizer.integrated.IntegratedCanvasVisualizer;
//
// A ten-node network topology
//
network Net5_moving
{
   parameters:
       bool hasVisualizer = default(false);
    types:
        channel C extends DatarateChannel
        {
            delay = uniform(0.01ms, 1s);
            datarate = uniform(1kbps, 100kbps);
        }
    submodules:
        visualizer: IntegratedCanvasVisualizer if hasVisualizer {
            parameters:
                @display("p=100,100");
        }
        
       rte[5]: Node {
           address=index;
            
        }
       
        // visualizer: IntegratedCanvasVisualizer if hasVisualizer {
          //  parameters:
            //    @display("p=100,100");
        //}
    connections:
        rte[1].port++ <--> C <--> rte[0].port++;
        rte[0].port++ <--> C <--> rte[2].port++;
        rte[4].port++ <--> C <--> rte[3].port++;
        rte[3].port++ <--> C <--> rte[1].port++;
        rte[3].port++ <--> C <--> rte[2].port++;
        rte[2].port++ <--> C <--> rte[4].port++;
        rte[1].port++ <--> C <--> rte[2].port++;
}

