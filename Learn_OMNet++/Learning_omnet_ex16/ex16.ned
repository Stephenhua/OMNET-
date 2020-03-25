
simple Txc_ex16
{
    parameters:
        @signal[arrival](type="long");
        @statistic[hopCount](title="hop count"; source="arrival"; record=vector,stats; interpolationmode=none);

        @display("i=block/routing");
    gates:
        inout gate[];
}

network TicToc_ex16
{
    parameters:
        int numCentralNodes = default(2);
    types:
            channel Channel extends ned.DelayChannel
            {
                delay=100ms;
                }
     submodules:
                    tic[numCentralNodes+4]:Txc_ex16;
     connections:
         tic[0].gate++ <--> Channel <--> tic[2].gate++;
         tic[1].gate++ <--> Channel <--> tic[2].gate++;
         
         
         for i=2..numCentralNodes+1
         {
             tic[i].gate++ <--> Channel <--> tic[i+1].gate++;
             }
      tic[numCentralNodes+2].gate++ <--> Channel <--> tic[numCentralNodes+1].gate++;
      tic[numCentralNodes+3].gate++ <--> Channel <--> tic[numCentralNodes +1].gate++;       
}