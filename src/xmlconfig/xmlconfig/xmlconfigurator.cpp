//////////////////////////////////////////////////////////////////////////////////////
///
/// \file xmlconfigurator.cpp
/// \brief This file contains definition for the XmlConfigurator class..
///
/// Author(s): John Reeder<br>
/// Copyright (c) 2010<br>
//////////////////////////////////////////////////////////////////////////////////////
#include "xmlconfig/xmlconfigurator.h"
#include "xmlconfig/xmlpathfinder.h"

using namespace std;
using namespace xmlconfig;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
XmlConfigurator::XmlConfigurator()
{
    mCacheInvalidFlag = true;
    mTimeLastModified = 0;
    mXmlDocPath = "";
    mConfiguratorName = "";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
XmlConfigurator::~XmlConfigurator()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The mConfiguratorName of this instance of XmlConfigurator.
///
////////////////////////////////////////////////////////////////////////////////////
string XmlConfigurator::GetName() const
{
    return mConfiguratorName;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The string for the XML path.
///
////////////////////////////////////////////////////////////////////////////////////
string XmlConfigurator::GetPath() const
{
    return mXmlDocPath;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function sets the path to the XML and reloads the XML.
///         
///   This will also cause the cache to be invalidated 
///
///   \param xmlPath A string containing the new xml path.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::SetPath(const string& xmlPath)
{
    mXmlDocPath = xmlPath;
    return Reload();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function initializes the xmlconfigurator. This must be called
///          in order to set the XML path.
///         
///   This will also cause the cache to be invalidated 
///
///   \param xmlPath A string containing the new xml path.
///   \param inName Configurator name.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::Load(const string& xmlPath , const string& inName )
{
    mXmlDocPath = xmlPath;
    mConfiguratorName = inName;
    return Reload();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function retrieves a boolean value from using an tokenized
///          string.  Only the first instance of the variable will be found.
///         
///   This will also cause the cache to be invalidated 
///
///   \param xmlPath A string containing the XML path to the desired XML 
///                  attribute you are looking for.  For example:
///                  mainNode.leafNode.leafNode2.var@myAttribute.
///   \param returnBool Boolean value retrieved.
///   \param optional If true, parameter don't spit out error message on failure.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::GetVar(const string& elemPath, bool& returnBool, const bool optional)
{
    string tmpString = "";
    if ( GetVar ( elemPath, tmpString, optional ) )
    {
        if ( tmpString == "1" || tmpString == "true" )
        {
            returnBool = true;
        }
        else
        {
            returnBool = false;
        }
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function retrieves a boolean value from using an tokenized
///          string.  Only the first instance of the variable will be found.
///         
///   This will also cause the cache to be invalidated 
///
///   \param xmlPath A string containing the XML path to the desired XML 
///                  attribute you are looking for.  For example:
///                  mainNode.leafNode.leafNode2.var@myAttribute.
///   \param returnInt Int value retrieved.
///   \param optional If true, parameter don't spit out error message on failure.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::GetVar(const string& elemPath, int& returnInt, const bool optional)
{
    string tmpString = "";
    if ( GetVar ( elemPath, tmpString, optional ) )
    {
        returnInt = atoi ( tmpString.c_str() );
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function retrieves a boolean value from using an tokenized
///          string.  Only the first instance of the variable will be found.
///         
///   This will also cause the cache to be invalidated 
///
///   \param xmlPath A string containing the XML path to the desired XML 
///                  attribute you are looking for.  For example:
///                  mainNode.leafNode.leafNode2.var@myAttribute.
///   \param returnDouble Double value retrieved.
///   \param optional If true, parameter don't spit out error message on failure.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::GetVar(const string& elemPath, double& returnDouble, const bool optional)
{
    string tmpString = "";
    if ( GetVar ( elemPath, tmpString, optional ) )
    {
        returnDouble = atof ( tmpString.c_str() );
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function retrieves a boolean value from using an tokenized
///          string.  Only the first instance of the variable will be found.
///         
///   This will also cause the cache to be invalidated 
///
///   \param xmlPath A string containing the XML path to the desired XML 
///                  attribute you are looking for.  For example:
///                  mainNode.leafNode.leafNode2.var@myAttribute.
///   \param returnString String value retrieved.
///   \param optional If true, parameter don't spit out error message on failure.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::GetVar(const string& elemPath, string& returnString, const bool optional)
{
    IsFileUpdated();
    bool returnVal = false;
    map<string, string>::iterator mapit;
    if ( ( mapit = mVariableCache.find ( elemPath ) ) != mVariableCache.end() )
    {
        returnVal = true;
        returnString = mapit->second;
    }
    else if (CheckXmlPointers(elemPath, returnString))
    {
        if (returnString.compare("NULL") != 0)
        {
            returnVal = true;
        }
        else
        {
            returnVal = false;
        }
    }
    else
    {
        vector<string> tokens;
        Tokenize ( elemPath, tokens );
        if ( tokens.size() > 0 )
        {
            TiXmlHandle hDoc ( &mXmlDoc );
            //This eats the root level element so it should not be part of the path.
            TiXmlElement *pCurrentElem = hDoc.FirstChildElement().ToElement();
            TiXmlElement *pNextElem = NULL;
            vector<string>::iterator it;

            bool haveAt = false;
           /* for(it = tokens.begin();
                it != tokens.end();
                it++)
            {
                if(it->find_first_of( '@') != string::npos)
                {
                    haveAt = true;
                    break;
                }
            }*/
			if (elemPath.find_first_of("@") != string::npos)
			{
				haveAt = true;
			}
            if(haveAt == false)
            {
                for(it = tokens.begin();
                    it != tokens.end();
                    it++)
                {
                    if(pCurrentElem)
                    {
                        pCurrentElem = pCurrentElem->FirstChildElement(it->c_str());
                    }
                }
                if(pCurrentElem && pCurrentElem->FirstChild())
                {
                    returnString = pCurrentElem->FirstChild()->Value();
                    return true;
                }
            }

            pCurrentElem = hDoc.FirstChildElement().ToElement();
            pNextElem = NULL;

            it = tokens.begin();
            int atPos;
            //This checks the token to see if it contains the @ symbol. 
            //It loops around adding the elements to the path until it is found
            while ( ( atPos = it->find_first_of ( '@' ) ) == string::npos )
            {
                pNextElem = pCurrentElem->FirstChildElement ( it->c_str() );
                if ( pNextElem )
                {
                    //This iterates through the tokens and moves forward down through the xml tree
                    it++;
                    pCurrentElem = pNextElem;
                    pNextElem = NULL;
                }
                else
                {
                    //This means the path given doesn't match the xml file. There is most likely an error in the xml.
                    if(!optional)
                    {
                        cout << GetTimeString() << ": Can't find element: " << it->c_str() << endl;
                    }
                    return false;
                }
            }
           
            vector<string> finalTokens;
            //This grabs the final element and the attribute being looked for
            Tokenize ( *it, finalTokens, "@" );
            //This returns the value held in the xml attribute. 
            TiXmlElement *tmpElem = pCurrentElem->FirstChildElement ( finalTokens[0].c_str() );
            string tmpRet = "";
            if(tmpElem)
            {
                const char* tmpstr = tmpElem->Attribute ( finalTokens[1].c_str() );
                bool found = false;
                if (tmpstr != NULL)
                {
                    tmpRet = tmpstr;
                    if (tmpRet.size() > 0 )
                    {
                        //checks to see if there is actually a value to return then places it in the return variable
                        returnString = tmpRet;
                        returnVal = true;
                        found = true;
                        //Adds the newly found value to the cache. 
                        mVariableCache[elemPath] = tmpRet;
                    }
                }

                if (!found)
                {
                    if(!optional)
                    {
                        cout << GetTimeString() << "Can't find attribute: " << finalTokens[1].c_str() << endl;
                    }
                }
            }
            else
            {
                if(!optional)
                {
                    cout << GetTimeString() << "Can't find element: " << finalTokens[0].c_str() << endl;
                }
            }

        }
    }
    return returnVal;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Utility method to find string tokens.
///
///   \param str The string to tokenize.
///   \param tokens Tokens found.
///   \param delimeters Delimeters for tokens.
///
////////////////////////////////////////////////////////////////////////////////////
void XmlConfigurator::Tokenize ( const string& str, vector<string>& tokens, const string& delimiters )
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of ( delimiters, 0 );
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of ( delimiters, lastPos );

    while ( string::npos != pos || string::npos != lastPos )
    {
        // Found a token, add it to the vector.
        tokens.push_back ( str.substr ( lastPos, pos - lastPos ) );
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of ( delimiters, pos );
        // Find next "non-delimiter"
        pos = str.find_first_of ( delimiters, lastPos );
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to reload XML file and cache when it has been updated on
///          disk.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::Reload()
{
    mXmlDoc = TiXmlDocument ( mXmlDocPath.c_str() );
    if ( !mXmlDoc.LoadFile() )
    {
        mCacheInvalidFlag = false;
        cout << GetTimeString() << ": xml file " << mXmlDocPath.c_str() << " failed to Load!!" << endl;
        cout << mXmlDoc.ErrorDesc() << " at row: " << mXmlDoc.ErrorRow() << " column: " << mXmlDoc.ErrorCol() << endl;
        return false;
    }
    TiXmlHandle hDoc ( &mXmlDoc );
    TiXmlElement *pElem;
    pElem = hDoc.FirstChildElement().Element();
    if ( !pElem )
    {
        cout << GetTimeString() << ": Error parsing " << mXmlDocPath.c_str() << endl;
        return false;
    }

    //Finds any xml file paths by looking for xml attributes. 
    XmlPathFinder xmlPaths;
    mXmlDoc.Accept(&xmlPaths);

    //Grab all the xml paths and check them to see if they are new or if they have been changed
    map<string, string> xmlPathsMap = xmlPaths.GetXmlMap();
    map<string, string>::iterator it = xmlPathsMap.begin();
    for (; it != xmlPathsMap.end(); it++)
    {
        if (mXmlPointers.find(it->first) != mXmlPointers.end())
        {
            if(mXmlPointers[it->first].GetPath() != it->second)
            {
                mXmlPointers[it->first].SetPath(it->second);
            }
        }
        else
        {
            mXmlPointers[it->first] = XmlConfigurator();
            mXmlPointers[it->first].Load(it->second,it->first);
        }
    }
    //hRoot = TiXmlHandle(pElem);

    //Grab the last edited time for the file to keep up with when it has been changed. 
    stat ( mXmlDocPath.c_str(), &mFileInfo );
    mTimeLastModified = mFileInfo.st_mtime;
    mVariableCache.clear();
    mCacheInvalidFlag = false;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks for file update and will invalidate cach if so.
///
///   \return True if file has been modified, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::IsFileUpdated()
{
    stat ( mXmlDocPath.c_str(), &mFileInfo );
    if ( mTimeLastModified < mFileInfo.st_mtime )
    {
        mCacheInvalidFlag = true;
        return Reload();
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current system time string.
///
////////////////////////////////////////////////////////////////////////////////////
string XmlConfigurator::GetTimeString()
{
    time_t rawTime;
    time ( &rawTime );
    return ctime ( &rawTime );
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks through the mXmlPointers to see if the elemPath points 
///          to another file if it does then the GetVar function of the other
///   file is called.
///
///   \param elemPath the path to the variable being looked for. 
///   \param returnString The string containing the value of the variable if 
///                       it is found.  If the elemPath is found but the GetVar 
///                       function returns false then returnString 
///                       will contain "NULL"
///
///   \return If true then the elemPath points to another file, if false then 
///           elemPath doesn't point to a different file.
///
////////////////////////////////////////////////////////////////////////////////////
bool XmlConfigurator::CheckXmlPointers(const string& elemPath, string& returnString)
{
    map<string,XmlConfigurator>::iterator xmlpathit = mXmlPointers.begin();
    bool returnVal = false;
    for (; xmlpathit != mXmlPointers.end(); xmlpathit++)
    {
        int sPos = 0;
        if ((sPos = elemPath.find(xmlpathit->first)) != string::npos)
        {
            string subPath = elemPath.substr(sPos, xmlpathit->first.size());
            string remPath = elemPath.substr(sPos + subPath.size(), string::npos);
            if (xmlpathit->second.GetVar(remPath, returnString ))
            {
                returnVal = true;
            }
            else
            {
                returnVal = true;
                returnString = "NULL";
            }
        }
    }
    return returnVal;
}

/* End of File */
