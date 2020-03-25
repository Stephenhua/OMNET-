//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple txc3
{
    parameters:
        bool sendMsgOnInit=default(false);
        int limit=default(10);
        @display("i=block/routing");
        gates:
            input in;
            output out;
}

simple Tic extends txc3
{
    parameters:
        @display("i=,cyan");
        sendMsgOnInit=true;
}

simple Toc extends txc3
{
    parameters:
        @display("i=,cyan");
        sendMsgOnInit=false;
}

network TicToc
{
    submodules:
        tic:Tic;
        toc:Toc;
        connections:
            tic.out --> {delay =100ms;} --> toc.in;
            tic.in<-- { delay = 100ms ;} <-- toc.out;
}