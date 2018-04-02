/*******************************************************************************
 * Copyright (c) The JETSCAPE Collaboration, 2017
 *
 * For the full list of contributors see AUTHORS.
 * Report issues at https://github.com/amajumder/JETSCAPE-COMP/issues
 * or via email to bugs.jetscape.org@gmail.com
 *
 * Distributed under the GNU General Public License 3.0 (GPLv3 or later).
 * See COPYING for details.
 ******************************************************************************/

#include "JetScapeXML.h"
#include "JetScapeLogger.h"
#include <stdlib.h>

using namespace std;

namespace Jetscape {

JetScapeXML* JetScapeXML::m_pInstance = NULL;

JetScapeXML* JetScapeXML::Instance()
{
  if (!m_pInstance)
    {
      INFO<<"Created JetScapeXML Instance";
      m_pInstance = new JetScapeXML();
    }
  
  return m_pInstance;
}

void JetScapeXML::OpenXMLFile()
{
  if (!xml_file_open)
    {
      xml_doc.LoadFile((char*) GetXMLFileName().c_str());
      INFO<<BOLDBLACK<<"Trying XML file : "<< GetXMLFileName();
      if (xml_doc.ErrorID()<1)
	{
	  INFO<<BOLDBLACK<<"Open XML file : "<< GetXMLFileName();
	  xml_root = (tinyxml2::XMLElement*) xml_doc.FirstChildElement("jetscape" );
	  
	  if (!xml_root)
	    {
	      WARN << "Not a valid JetScape XML file!";
	      exit(-1);
	    }
	}
      else
	{
	  WARN << "XML file not found/not properly opened! Error code : "<<xml_doc.ErrorID();
	  exit(-1);
	}
      xml_file_open=true;
    }
}

void JetScapeXML::OpenXMLFile(string m_name)
{
  SetXMLFileName(m_name);
  OpenXMLFile();
}

} // end namespace Jetscape
