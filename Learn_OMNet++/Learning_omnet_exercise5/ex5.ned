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
simple Ex5
{
    parameters:
        volatile double delayTime @ unit(s);
        @display("i=block/routing");
        gates:
            input in;
            output out;
}

network Net_ex5
{
    submodules:
        tic:Ex5
        {
            parameters:
                @display("i=,cyan");
        }
        toc:Ex5
        {
            parameters:
                @display("i=,gold");
        }
        connections:
            tic.out --> {delay = 100ms; } --> toc.in;
            tic.in<-- { delay =100ms ;} <-- toc.out;
}