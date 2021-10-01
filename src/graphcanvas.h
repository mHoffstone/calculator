#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <vector>
#include <string>

#include "expression.h"
#include "vector.h"

#include <wx/wx.h>

class GraphCanvas : public wxWindow{
	public:
		GraphCanvas(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size);
		virtual ~GraphCanvas();
		
		void add(int i, const std::string& str);
		void set(int i, const std::string& str);
		void remove(int i);
		
	private:
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		
		const wxFont font = wxFont(11, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		
		vec2d pan;
		vec2d zoom;
		wxSize size;
		
		wxPoint oldMouse;
		
		vec2d toScreen(vec2d v);
		vec2d toWorld(vec2d v);
		
		std::vector<std::string> inputs;
		std::vector<Expression*> expressions;
		std::vector<std::string> outputs;
		
		void OnMouseMove(wxMouseEvent& event);
		void OnZoom(wxMouseEvent& event);
		
		wxDECLARE_EVENT_TABLE();
};

#endif // GRAPHCANVAS_H
