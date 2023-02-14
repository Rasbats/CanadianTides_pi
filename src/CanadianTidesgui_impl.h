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

#ifndef _CALCULATORGUI_IMPL_H_
#define _CALCULATORGUI_IMPL_H_

#ifdef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "CanadianTidesgui.h"
#include "CanadianTides_pi.h"
#include "NavFunc.h"
#include "tidetable.h"
#include "tinyxml.h"
#include "wx/stdpaths.h"
#include "wx/msgdlg.h"
#include <wx/gdicmn.h>

#include "json/reader.h"
#include "json/writer.h"

#include "ocpn_plugin.h"
#include "pidc.h"

#include "TexFont.h"


#include <map>
#include <list>
#include <vector>

#include "wx/defs.h"

#ifdef __OCPN__ANDROID__
#include <qopengl.h>
#include "gl_private.h"
#endif

class PlugIn_ViewPort;
class wxBoundingBox;
class piDC;

using namespace std;

struct TidalEvent
{
	wxString EventType;
	wxString DateTime;
	wxString Height;
};

struct myPort
{
	wxString Name;
	wxString DownloadDate;
	wxString Id;
	double coordLat;
	double coordLon;
	list<TidalEvent>tidalevents;
};

class CanadianTides_pi;
class Position;
class TideTable;

class Dlg : public DlgDef
{
public:
	Dlg(CanadianTides_pi &_CanadianTides_pi, wxWindow* parent);
	~Dlg();

#ifdef __OCPN__ANDROID__
    void OnMouseEvent( wxMouseEvent& event );
#endif

        void OnDownload( wxCommandEvent& event );	
		void OnGetSavedTides(wxCommandEvent& event);
		void DoRemovePortIcons(wxCommandEvent& event);
		void DoRemoveAllPortIcons(wxCommandEvent& event);

		void OnInformation(wxCommandEvent& event);
        void Addpoint(TiXmlElement* Route, wxString ptlat, wxString ptlon, wxString ptname, wxString ptsym, wxString pttype);	
			
		CanadianTides_pi *plugin; 

		wxString rte_start;
	    wxString rte_end;
	
		void getPort(double m_lat, double m_lon);
		wxString m_default_configuration_path;
		void AutoSizeHeader(wxListCtrl *const list_ctrl);

		CanadianTides_pi &m_CanadianTides_pi;
		wxString StandardPath();

		list<myPort>myports;
		list<myPort>mySavedPorts;

		myPort mySavedPort;

		void SetViewPort(PlugIn_ViewPort *vp);
		bool RenderOverlay(piDC &dc, PlugIn_ViewPort &vp);
		void DrawAllStationIcons(PlugIn_ViewPort *BBox, bool bRebuildSelList, bool bforce_redraw_icons, bool bdraw_mono_for_mask);
		void DrawAllSavedStationIcons(PlugIn_ViewPort *BBox, bool bRebuildSelList,
			bool bforce_redraw_icons, bool bdraw_mono_for_mask);
		void DrawOLBitmap(const wxBitmap &bitmap, wxCoord x, wxCoord y, bool usemask);
		void DrawGLLabels(Dlg *pof, wxDC *dc, PlugIn_ViewPort *vp,
			wxImage &imageLabel, double myLat, double myLon, int offset);
		wxImage &DrawGLTextString(wxString myText);

		void DrawLine(double x1, double y1, double x2, double y2,
			const wxColour &color, double width);

		wxBitmap m_stationBitmap;


		TideTable* tidetable;
		bool b_usingSavedPorts;
		bool b_clearSavedIcons;
		bool b_clearAllIcons;
		void OnShow(void);
		void OnTest(wxString thePort);
		void RemoveSavedPort(wxString myStation);
		void RemoveAllSavedPorts();

		wxMessageDialog* mdlg;
		
		PlugIn_ViewPort  *vp;
		PlugIn_ViewPort  *m_vp;

		piDC *m_dc;
		bool b_HideButtons;

private:
	
	
	wxString m_titlePortName;
	
	list<TidalEvent>myevents;
	list<TidalEvent>mySavedEvents;

	myPort SavePortTidalEvents(list<TidalEvent>myevents, string portId);
	void SaveTidalEventsToXml(list<myPort>myPorts);

	list<myPort> LoadTidalEventsFromXml();

	double AttributeDouble(TiXmlElement *e, const char *name, double def);
	wxString GetDateStringNow();	

	void getHWLW(string id);
	wxString getPortId(double m_lat, double m_lon);
	wxString getSavedPortId(double m_lat, double m_lon);
	wxString ProcessDate(wxString myLongDate);
	
	void OnShowSavedPortTides(wxString thisPortId);
	void OnClose( wxCloseEvent& event );

	
    double lat1, lon1, lat2, lon2;
    bool error_found;
    bool dbg;

	wxString     m_gpx_path;	
	wxDateTime m_dtNow;

	wxFont *pTCFont;
	wxColour m_text_color;
	std::map < double, wxImage > m_labelCache;
	std::map < wxString, wxImage > m_labelCacheText;

	
	
};


class Position
{
public:

    wxString lat, lon, wpt_num;
    Position *prev, *next; /* doubly linked circular list of positions */
    int routepoint;

};

class GetTidalEventDialog : public wxDialog
{
public:

	GetTidalEventDialog(wxWindow * parent, wxWindowID id, const wxString & title,
		const wxPoint & pos = wxDefaultPosition,
		const wxSize & size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE);

	wxListView * dialogText;
	wxString GetText();

	wxStaticBoxSizer* m_pListSizer;
	wxScrolledWindow *itemDialog1;
	wxStaticBox* itemStaticBoxSizer14Static;

	wxButton*     m_OKButton;

private:

	void OnOk(wxCommandEvent & event);

};

#endif
