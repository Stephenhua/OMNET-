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
// Interface for network traffic generator modules that
// can be used in Node.
//
// 一个简单的APP接口，相当于一个箱子，里面放的什么东西并不知道
moduleinterface IApp
{
    parameters:
        int address;  // local node address
    gates:
        input in;
        output out;
}


