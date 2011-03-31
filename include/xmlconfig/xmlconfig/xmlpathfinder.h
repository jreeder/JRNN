//////////////////////////////////////////////////////////////////////////////////////
///
/// \file xmlpathfinder.h
/// \brief This file contains a TinyXML Node Visitor class to support
///        XmlConfigurator.
///
/// Author(s): John Reeder<br>
/// Copyright (c) 2010<br>
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _XMLPATHFINDER_H
#define _XMLPATHFINDER_H

#include "tinyxml/tinyxml.h"
#include <map>

namespace xmlconfig
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class XmlPathFinder
    ///   \brief TiXmlVisitor implementation to help find data within XML.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class XmlPathFinder : public TiXmlVisitor
    {
        public:
            XmlPathFinder()
            {
                mXmlAttribute = "xml";
            }
            ~XmlPathFinder() {}
            std::map<std::string, std::string> GetXmlMap() const
            {
                return mXmlMap;
            }
            virtual bool VisitEnter ( const TiXmlElement &element, const TiXmlAttribute *firstAttribute )
            {
                mParentElements.push_back ( element.Value() );
                if ( firstAttribute && mXmlAttribute.compare ( firstAttribute->Name() ) == 0 )
                {
                    mXmlMap[GetElemPath ( mParentElements ) ] = firstAttribute->Value();
                }
                return true;
            }
            virtual bool VisitExit ( const TiXmlElement &element )
            {
                mParentElements.pop_back();
                return true;
            }
        private:
            std::string GetElemPath (const std::vector<std::string>& elements )
            {
                std::vector<std::string>::const_iterator it;
                std::string outstring = "";
                it = elements.begin();
                it++;
                for (; it < elements.end(); it++ )
                {
                    outstring += ( *it );
                    outstring += ".";
                }
                return outstring;
            }
            std::map<std::string, std::string> mXmlMap; ///<  Map of XML Data
            std::vector<std::string> mParentElements;   ///<  Names of parent elements.
            std::string mXmlAttribute;                  ///<  XML Attribute string.
    };
}

#endif 

/*  End of File */
