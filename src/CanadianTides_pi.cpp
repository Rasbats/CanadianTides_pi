/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  CanadianTides Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2019 by Mike Rossiter                                *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "CanadianTides_pi.h"
#include "CanadianTidesgui_impl.h"
#include "CanadianTidesgui.h"

#include <wx/stdpaths.h>



class CanadianTides_pi;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new CanadianTides_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//   CanadianTides PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#define CanadianTides_TOOL_POSITION    -1 
#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

CanadianTides_pi::CanadianTides_pi(void *ppimgr)
      :opencpn_plugin_116 (ppimgr)
{
		// Create the PlugIn icons
	initialize_images();

	wxFileName fn;

	wxString path = GetPluginDataDir("CanadianTides_pi");
	fn.SetPath(path);
	fn.AppendDir("data");
	fn.SetFullName("CanadianTides_panel_icon.png");

	path = fn.GetFullPath();
    
    wxInitAllImageHandlers();

	wxLogDebug(wxString("Using icon path: ") + path);
	if (!wxImage::CanRead(path)) {
		wxLogDebug("Initiating image handlers.");
		wxInitAllImageHandlers();
	}

	wxImage panelIcon(path);

	if (panelIcon.IsOk())
		m_panelBitmap = wxBitmap(panelIcon);
	else
		wxLogMessage(_("    CanadianTides panel icon has NOT been loaded"));



	m_bShowCanadianTides = false;
}

CanadianTides_pi::~CanadianTides_pi(void)
{
     delete _img_CanadianTides;
     
}

int CanadianTides_pi::Init(void)
{
      AddLocaleCatalog("opencpn-CanadianTides_pi");

      // Set some default private member parameters
      m_route_dialog_x = 0;
      m_route_dialog_y = 0;
      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      //    This PlugIn needs a toolbar icon, so request its insertion
	  if (m_bCanadianTidesShowIcon) {

#ifdef PLUGIN_USE_SVG
		  m_leftclick_tool_id = InsertPlugInToolSVG("CanadianTides", _svg_CanadianTides, _svg_CanadianTides, _svg_CanadianTides_toggled,
			  wxITEM_CHECK, _("CanadianTides"), "", NULL, CanadianTides_TOOL_POSITION, 0, this);
#else
		  m_leftclick_tool_id = InsertPlugInTool("", _img_CanadianTides, _img_CanadianTides, wxITEM_CHECK,
			  _("CanadianTides"), "", NULL,
			  CanadianTides_TOOL_POSITION, 0, this);
#endif
	  }
	wxMenu dummy_menu;
	m_position_menu_id = AddCanvasContextMenuItem

	(new wxMenuItem(&dummy_menu, -1, _("Select UK Tidal Station")), this);
	SetCanvasContextMenuItemViz(m_position_menu_id, false);

     m_pDialog = NULL;	 
	
	

      return (WANTS_OVERLAY_CALLBACK |
              WANTS_OPENGL_OVERLAY_CALLBACK |		      
		      WANTS_CURSOR_LATLON      |
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_CONFIG            
           );
}

bool CanadianTides_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetCalculatorDialogX(p.x);
            SetCalculatorDialogY(p.y);
            m_pDialog->Close();

            delete m_pDialog;
            m_pDialog = NULL;

			m_bShowCanadianTides = false;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowCanadianTides );

      }	
    
    SaveConfig();
    
    return true;
}

int CanadianTides_pi::GetAPIVersionMajor()
{
      return atoi(API_VERSION);
}

int CanadianTides_pi::GetAPIVersionMinor()
{
    std::string v(API_VERSION);
    size_t dotpos = v.find('.');
    return atoi(v.substr(dotpos + 1).c_str());
}

int CanadianTides_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int CanadianTides_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

wxBitmap *CanadianTides_pi::GetPlugInBitmap()
{
      return &m_panelBitmap;
}

wxString CanadianTides_pi::GetCommonName()
{
      return _("CanadianTides");
}


wxString CanadianTides_pi::GetShortDescription()
{
      return _("CanadianTides");
}

wxString CanadianTides_pi::GetLongDescription()
{
      return _("Downloads UKHO Tidal Data for UK ports");
}

int CanadianTides_pi::GetToolbarToolCount(void)
{
      return 1;
}

void CanadianTides_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pDialog)
            return;

      DimeWindow(m_pDialog);
}

void CanadianTides_pi::OnToolbarToolCallback(int id)
{
    
	if(NULL == m_pDialog)
      {
           
		    m_pDialog = new Dlg(*this, m_parent_window);
            m_pDialog->plugin = this;
            m_pDialog->Move(wxPoint(m_route_dialog_x, m_route_dialog_y));

			wxFileName fn;
			wxString path;

			path = GetPluginDataDir("CanadianTides_pi");
			fn.SetPath(path);
			fn.AppendDir(_T("data"));
			fn.SetFullName("station_icon.png");

			path = fn.GetFullPath();

			wxLogDebug(wxString("Using station icon path: ") + path);
			if (!wxImage::CanRead(path)) {
				wxLogDebug("Initiating image handlers.");
				wxInitAllImageHandlers();
			}
			
			wxImage stationIcon(path);

			if (stationIcon.IsOk())
				m_pDialog->m_stationBitmap = wxBitmap(stationIcon);
			else
				wxLogMessage(_("CanadianTides:: station bitmap has NOT been loaded"));	

			m_pDialog->b_clearAllIcons = false;
			m_pDialog->b_clearSavedIcons = false;
						
      }

	  m_pDialog->Fit();
	  //Toggle 
	  m_bShowCanadianTides = !m_bShowCanadianTides;	  

      //    Toggle dialog? 
      if(m_bShowCanadianTides) {		  
          m_pDialog->Show();
		  m_pDialog->b_clearAllIcons = true;
		  m_pDialog->b_clearSavedIcons = false;

	  }
	  else {		 
		  m_pDialog->Hide();
		  m_pDialog->b_clearAllIcons = true;
		  m_pDialog->b_clearSavedIcons = true;
	  }
      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowCanadianTides );

      RequestRefresh(m_parent_window); // refresh main window
}

void CanadianTides_pi::OnCanadianTidesDialogClose()
{
	m_pDialog->b_clearSavedIcons = true;
	m_pDialog->b_clearAllIcons = true;
	m_bShowCanadianTides = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowCanadianTides );
    m_pDialog->Hide();
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

}


bool CanadianTides_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/CanadianTides_pi" ) );
			 pConf->Read ( _T( "ShowCanadianTidesIcon" ), &m_bCanadianTidesShowIcon, 1 );
           
            m_route_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_route_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );
         
            if((m_route_dialog_x < 0) || (m_route_dialog_x > m_display_width))
                  m_route_dialog_x = 5;
            if((m_route_dialog_y < 0) || (m_route_dialog_y > m_display_height))
                  m_route_dialog_y = 5;
            return true;
      }
      else
            return false;
}

bool CanadianTides_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/CanadianTides_pi" ) );
			pConf->Write ( _T ( "ShowCanadianTidesIcon" ), m_bCanadianTidesShowIcon );
          
            pConf->Write ( _T ( "DialogPosX" ),   m_route_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_route_dialog_y );
            
            return true;
      }
      else
            return false;
}

bool CanadianTides_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
	if (!m_pDialog)
		return false;

	piDC pidc(dc);
	m_pDialog->RenderOverlay(pidc, *vp);
	return true;
}

bool CanadianTides_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
	if (!m_pDialog) 
		return false;

	//m_pDialog->SetViewPort(vp);
	piDC piDC;
    glEnable( GL_BLEND );
    piDC.SetVP(vp);

	m_pDialog->RenderOverlay(piDC, *vp);
	return true;
}


void CanadianTides_pi::OnContextMenuItemCallback(int id)
{
	if (!m_pDialog)
		return;
	
	if (id == m_position_menu_id) {
		m_cursor_lat = GetCursorLat();
		m_cursor_lon = GetCursorLon();
		m_pDialog->getPort(m_cursor_lat, m_cursor_lon);
	}	
}

void CanadianTides_pi::SetCursorLatLon(double lat, double lon)
{
	m_cursor_lat = lat;
	m_cursor_lon = lon;
}
