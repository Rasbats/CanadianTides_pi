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

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include "CanadianTides_pi.h"
#include "CanadianTidesgui_impl.h"
#include "CanadianTidesgui.h"
#include "plug_utils.h"
#include <wx/stdpaths.h>
#include "config.h"
#include "icons.h"
#include "ocpn_plugin.h"
#include "icons.h"

class CanadianTides_pi;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin *create_pi(void *ppimgr) {
  return new CanadianTides_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *p) { delete p; }

//---------------------------------------------------------------------------------------------------------
//
//   CanadianTides PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

CanadianTides_pi::CanadianTides_pi(void *ppimgr)
    : opencpn_plugin_118(ppimgr),

      m_bCanadianTidesShowIcon(false)

{
  // Create the PlugIn icons
  initialize_images();
  auto icon_path = GetPluginIcon("CanadianTides_panel_icon", PKG_NAME);
  if (icon_path.type == IconPath::Type::Svg)
    m_panel_bitmap = LoadSvgIcon(icon_path.path.c_str());
  else if (icon_path.type == IconPath::Type::Png)
    m_panel_bitmap = LoadPngIcon(icon_path.path.c_str());
  else  // icon_path.type == NotFound
    wxLogMessage("Cannot find icon for basename: %s",
                 "CanadianTides_panel_icon");
  if (m_panel_bitmap.IsOk())
    wxLogMessage("CanadianTides_pi::, bitmap OK");
  else
    wxLogMessage("CanadianTides_pi::, bitmap fail");

  m_bShowCanadianTides = false;
}

CanadianTides_pi::~CanadianTides_pi(void) { delete _img_CanadianTides; }

int CanadianTides_pi::Init(void) {
  AddLocaleCatalog("opencpn-CanadianTides_pi");

  // Set some default private member parameters
  m_route_dialog_x = 0;
  m_route_dialog_y = 0;
  ::wxDisplaySize(&m_display_width, &m_display_height);

  //    Get a pointer to the opencpn display canvas, to use as a parent for the
  //    POI Manager dialog
  m_parent_window = GetOCPNCanvasWindow();

  //    Get a pointer to the opencpn configuration object
  m_pconfig = GetOCPNConfigObject();

  //    And load the configuration items
  LoadConfig();

  //    This PlugIn needs a toolbar icon, so request its insertion

  auto icon = GetPluginIcon("CanadianTides_pi", PKG_NAME);
  auto toggled_icon = GetPluginIcon("CanadianTides_pi_toggled", PKG_NAME);
  //    This PlugIn needs a toolbar icon, so request its insertion
  if (m_bCanadianTidesShowIcon) {
    if (icon.type == IconPath::Type::Svg)
      m_leftclick_tool_id = InsertPlugInToolSVG(
          "CanadianTides", icon.path, icon.path, toggled_icon.path,
          wxITEM_CHECK, "CanadianTides", "", nullptr,
          CanadianTides_TOOL_POSITION, 0, this);
    else if (icon.type == IconPath::Type::Png) {
      auto bitmap = LoadPngIcon(icon.path.c_str());
      m_leftclick_tool_id =
          InsertPlugInTool("", &bitmap, &bitmap, wxITEM_CHECK, "CanadianTides",
                           "", nullptr, CanadianTides_TOOL_POSITION, 0, this);
    }
  }
  wxMenu dummy_menu;
  m_position_menu_id = AddCanvasContextMenuItem

      (new wxMenuItem(&dummy_menu, -1, _("Select Canadian Tidal Station")),
       this);
  SetCanvasContextMenuItemViz(m_position_menu_id, false);

  m_pDialog = nullptr;

  return (WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK |
          WANTS_CURSOR_LATLON | WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL |
          WANTS_CONFIG);
}

bool CanadianTides_pi::DeInit(void) {
  //    Record the dialog position
  if (m_pDialog) {
    // Capture dialog position
    wxPoint p = m_pDialog->GetPosition();
    SetCanadianTidesDialogX(p.x);
    SetCanadianTidesDialogY(p.y);
    m_pDialog->Close();

    delete m_pDialog;
    m_pDialog = NULL;

    m_bShowCanadianTides = false;
    SetToolbarItemState(m_leftclick_tool_id, m_bShowCanadianTides);
  }

  SaveConfig();

  return true;
}

int CanadianTides_pi::GetAPIVersionMajor() { return atoi(API_VERSION); }

int CanadianTides_pi::GetAPIVersionMinor() {
  std::string v(API_VERSION);
  size_t dotpos = v.find('.');
  return atoi(v.substr(dotpos + 1).c_str());
}

int CanadianTides_pi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }

int CanadianTides_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

int CanadianTides_pi::GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }

int CanadianTides_pi::GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; };

const char *CanadianTides_pi::GetPlugInVersionPre() { return PKG_PRERELEASE; }

const char *CanadianTides_pi::GetPlugInVersionBuild() { return PKG_BUILD_INFO; }

wxBitmap *CanadianTides_pi::GetPlugInBitmap() { return &m_panel_bitmap; }

wxString CanadianTides_pi::GetCommonName() { return PLUGIN_API_NAME; }

wxString CanadianTides_pi::GetShortDescription() { return PKG_SUMMARY; }


wxString CanadianTides_pi::GetLongDescription() { return PKG_DESCRIPTION; }

int CanadianTides_pi::GetToolbarToolCount(void) { return 1; }

void CanadianTides_pi::SetColorScheme(PI_ColorScheme cs) {
  if (!m_pDialog) return;

  DimeWindow(m_pDialog);
}

void CanadianTides_pi::OnToolbarToolCallback(int id) {
  if (!m_pDialog) {
    m_pDialog = new Dlg(m_parent_window);
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
  // Toggle
  m_bShowCanadianTides = !m_bShowCanadianTides;

  //    Toggle dialog?
  if (m_bShowCanadianTides) {
    m_pDialog->Show();
    m_pDialog->b_clearAllIcons = true;
    m_pDialog->b_clearSavedIcons = false;

  } else {
    m_pDialog->Hide();
    m_pDialog->b_clearAllIcons = true;
    m_pDialog->b_clearSavedIcons = true;
  }
  // Toggle is handled by the toolbar but we must keep plugin manager b_toggle
  // updated to actual status to ensure correct status upon toolbar rebuild
  SetToolbarItemState(m_leftclick_tool_id, m_bShowCanadianTides);


  RequestRefresh(m_parent_window);  // refresh main window
}

void CanadianTides_pi::OnCanadianTidesDialogClose() {
  m_pDialog->b_clearSavedIcons = true;
  m_pDialog->b_clearAllIcons = true;
  m_bShowCanadianTides = false;
  SetToolbarItemState(m_leftclick_tool_id, m_bShowCanadianTides);
  m_pDialog->Hide();
  SaveConfig();

  RequestRefresh(m_parent_window);  // refresh main window
}

bool CanadianTides_pi::LoadConfig(void) {
  wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

  if (pConf) {
    pConf->SetPath(_T( "/Settings/CanadianTides_pi" ));
    pConf->Read(_T( "ShowCanadianTidesIcon" ), &m_bCanadianTidesShowIcon, 1);

    m_route_dialog_x = pConf->Read(_T ( "DialogPosX" ), 20L);
    m_route_dialog_y = pConf->Read(_T ( "DialogPosY" ), 20L);

    if ((m_route_dialog_x < 0) || (m_route_dialog_x > m_display_width))
      m_route_dialog_x = 5;
    if ((m_route_dialog_y < 0) || (m_route_dialog_y > m_display_height))
      m_route_dialog_y = 5;
    return true;
  } else
    return false;
}

bool CanadianTides_pi::SaveConfig(void) {
  wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

  if (pConf) {
    pConf->SetPath(_T ( "/Settings/CanadianTides_pi" ));
    pConf->Write(_T ( "ShowCanadianTidesIcon" ), m_bCanadianTidesShowIcon);

    pConf->Write(_T ( "DialogPosX" ), m_route_dialog_x);
    pConf->Write(_T ( "DialogPosY" ), m_route_dialog_y);

    return true;
  } else
    return false;
}

bool CanadianTides_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp) {
  if (!m_pDialog) return false;

  piDC pidc(dc);
  m_pDialog->RenderOverlay(pidc, *vp);
  return true;
}

bool CanadianTides_pi::RenderGLOverlay(wxGLContext *pcontext,
                                       PlugIn_ViewPort *vp) {
  if (!m_pDialog) return false;

  // m_pDialog->SetViewPort(vp);
  piDC piDC;
  glEnable(GL_BLEND);
  piDC.SetVP(vp);

  m_pDialog->RenderOverlay(piDC, *vp);
  return true;
}

void CanadianTides_pi::OnContextMenuItemCallback(int id) {
  if (!m_pDialog) return;

  if (id == m_position_menu_id) {
    m_cursor_lat = GetCursorLat();
    m_cursor_lon = GetCursorLon();
    m_pDialog->getPort(m_cursor_lat, m_cursor_lon);
  }
}

void CanadianTides_pi::SetCursorLatLon(double lat, double lon) {
  m_cursor_lat = lat;
  m_cursor_lon = lon;
}
