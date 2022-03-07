/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  CanadianTides Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2019 by Mike Rossiter                                   *
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

#ifndef _CALCULATOR_PI_H_
#define _CALCULATOR_PI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>

#include "ocpn_plugin.h" //Required for OCPN plugin functions
#include "icons.h"
#include "CanadianTidesgui_impl.h"

#include "pidc.h"

#include "config.h"

class piDC;
class Dlg;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define CanadianTides_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class CanadianTides_pi : public opencpn_plugin_116
{
public:
      CanadianTides_pi(void *ppimgr);
	   ~CanadianTides_pi(void);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();
	  


//    The required override PlugIn Methods
      int GetToolbarToolCount(void);
      void OnToolbarToolCallback(int id);
     

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);


//    The override PlugIn Methods
	void OnContextMenuItemCallback(int id);
	void SetCursorLatLon(double lat, double lon);


//    Other public methods
      void SetCalculatorDialogX         (int x){ m_route_dialog_x = x;};
      void SetCalculatorDialogY         (int x){ m_route_dialog_y = x;};
      void SetCalculatorDialogWidth     (int x){ m_route_dialog_width = x;};
      void SetCalculatorDialogHeight    (int x){ m_route_dialog_height = x;};      
	  bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
	  bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
	  void OnCanadianTidesDialogClose();
	  double GetCursorLon(void) { return m_cursor_lon; }
	  double GetCursorLat(void) { return m_cursor_lat; }
	  
	  int m_position_menu_id;

private:
      
	

	double m_cursor_lon;
	double m_cursor_lat;

	  void OnClose( wxCloseEvent& event );
	  CanadianTides_pi *plugin;
	  
	  wxFileConfig      *m_pconfig;
      wxWindow          *m_parent_window;
      bool              LoadConfig(void);
      bool              SaveConfig(void);
      Dlg               *m_pDialog;
      int               m_route_dialog_x, m_route_dialog_y,m_route_dialog_width,m_route_dialog_height;
      int               m_display_width, m_display_height;      
      int               m_leftclick_tool_id;
      bool              m_ShowHelp,m_bCaptureCursor,m_bCaptureShip;
      double m_ship_lon,m_ship_lat;

	  bool             m_bCanadianTidesShowIcon;
	  bool             m_bShowCanadianTides;
	  wxBitmap         m_panelBitmap;
};

#endif
