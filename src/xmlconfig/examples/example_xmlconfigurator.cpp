//////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_xmlconfigurator.h
/// \brief Simple example of how to use XmlConfigurator.
///
/// Author(s): John Reeder<br>
/// Copyright (c) 2010<br>
//////////////////////////////////////////////////////////////////////////////////////
#include "xmlconfig/xmlconfigurator.h"

using namespace std;
using namespace xmlconfig;

int main(int argc, char* argv[])
{
    cout << "starting ..." << endl;
    string xmlPath = "./main.xml";
    XmlConfigurator xmlconf;
    xmlconf.Load(xmlPath, "main");

    int flaredepth;
    int machdepth;
    int machtimeout;
    double flareangle;
    double machangle;
    string flarename = "";

    xmlconf.GetVar("missions.flare.vars@flaredepth",flaredepth);
    xmlconf.GetVar("missions.flare.vars@flareangle",flareangle);
    xmlconf.GetVar("missions.flare.vars@missionname",flarename);

    xmlconf.GetVar("missions.machinegunnest.vars@machdepth",machdepth);
    xmlconf.GetVar("missions.machinegunnest.vars@machtimeout",machtimeout);
    xmlconf.GetVar("missions.machinegunnest.vars@machangle",machangle);

    cout << "Flare Angle: " << flareangle << endl;
    cout << "Flare Depth: " << flaredepth << endl;
    cout << "Mission Name: " << flarename << endl;
    cout << "Machine Gun Depth: " << machdepth << endl;
    cout << "Machine Gun Time Out: " << machtimeout << endl;
    cout << "Machine Gun Angle: " << machangle << endl;


    cout.flush();

    return 0;
}

/*  End of File */

