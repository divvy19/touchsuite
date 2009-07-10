//////////////////////////////////////////////////////////////////////////////////////////
// Project description
// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// Name: TouchlibCOMwrapper
// Purpose: A touchlib wrapper for C#
//
// Author: Laurence Muller (aka Falcon4ever)
// Contact: info@falcon4ever.com
// Site: www.multigesture.net
// Licensetype: GNU General Public License v2
//
///////////////////////////////////////////////////////////////////////////////

The Wrapper makes use of COM.

The TouchlibCOMwrapper
======================

I've included a compiled build of TouchlibCOMwrapper.idl
If you made some changes in the touchlib event handler you might need to modify the idl aswell.

To compile the idl you will need to run a command window with the Visualstudio environment.
Command: midl.exe TouchlibCOMwrapper.idl
This will produce a file called TouchlibCOMwrapper.tlb

The wrapper will include this file (TouchlibCOMwrapper.h)


However you will also need to register the file (to use it in C#)
To register a file run the following command:
regtlib.exe TouchlibCOMwrapper.tlb

(or run the batch file)


If you don't register this file, you will NOT be able to add the file as a reference in your C# project.
















