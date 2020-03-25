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
simple Tic_ex4
{
    parameters:
        @display("i=block/routing");
    gates:
            output out;
            input in;
}

simple tic extends Tic_ex4
{
    parameters:
        @dispaly("i=,cyan");
}

simple toc extends Tic_ex4
{
    parameters:
        @display("i=,gold");
}

network NET_ex4
{
    submodules:
        tic_1 : tic;
        toc_1 : toc;
    connections:
            tic_1.out --> { delay=10ms; } --> toc_1.in;
            tic_1.in <-- { delay =10ms;  } <-- toc_1.out;
}