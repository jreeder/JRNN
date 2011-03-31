//////////////////////////////////////////////////////////////////////////////////////
///
/// \file xmlconfigurator.h
/// \brief This file contains definition for the XmlConfigurator class..
///
/// Author(s): John Reeder<br>
/// Copyright (c) 2010<br>
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _XMLCONFIGURATOR_H
#define _XMLCONFIGURATOR_H

#include "tinyxml/tinyxml.h"
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

namespace xmlconfig
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class XmlConfigurator
    ///   \brief Interface to cache contents of an XML file in memory for
    ///          quick look up using helper methods.
    ///
    ///   Helper methods (GetVar) allow you to lookup attributes within nested
    ///   XML nodes quickly.  Configurator also checks for file update changes
    ///   and will reload.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class XmlConfigurator
    {
    public:
        XmlConfigurator();
        ~XmlConfigurator();
        std::string GetName() const;
        std::string GetPath() const;
        bool SetPath(const std::string& xmlPath);
        bool Load(const std::string& xmlPath, const std::string& inName);
        bool GetVar(const std::string& elemPath, bool& returnBool, const bool optional = false);
        bool GetVar(const std::string& elemPath, int& returnInt, const bool optional = false);
        bool GetVar(const std::string& elemPath, double& returnDouble, const bool optional = false);
        bool GetVar(const std::string& elemPath, std::string& returnString, const bool optional = false);
        TiXmlDocument* GetTinyXML() { return &mXmlDoc; }
        const TiXmlDocument* GetTinyXML() const { return &mXmlDoc; }
        static void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ".");
    private:
        bool Reload();
        bool IsFileUpdated();
        std::string GetTimeString();
        bool CheckXmlPointers(const std::string& elemPath, std::string& returnString);

        std::string mXmlDocPath;                                ///<  Document path.
        std::map<std::string, std::string> mVariableCache;      ///<  Variable cache.
        std::map<std::string, XmlConfigurator> mXmlPointers;    ///<  Map of pointers to different configurators from nested files.
        time_t mTimeLastModified;                               ///<  Time of last file modification.
        bool mCacheInvalidFlag;                                 ///<  Flag for invalid cache due to file change.
        struct stat mFileInfo;                                  ///<  File information for tim stamps.
        TiXmlDocument mXmlDoc;                                  ///<  XML Document handle.
        std::string mConfiguratorName;                          ///<  Name of configurator or file loaded.
    };
}

#endif

/*  End of File */
