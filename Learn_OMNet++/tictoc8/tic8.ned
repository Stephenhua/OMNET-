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
simple Tic8
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple Toc8
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
        output out;
}

network Tictoc8
{
    @display("bgb=298,181");
    submodules:
        tic: Tic8 {
            parameters:
                @display("i=,cyan");
        }
        toc: Toc8 {
            parameters:
                @display("i=,gold;p=198,115");

        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
