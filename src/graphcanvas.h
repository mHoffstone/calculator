#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <vector>
#include <string>

#include <wx/wx.h>

class GraphCanvas : public wxWindow{
	public:
		GraphCanvas(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size);
		virtual ~GraphCanvas();

		std::vector<std::string> inputs;

	private:
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnEraseBackground(wxEraseEvent& event);

		const wxFont font = wxFont(11, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		float xPan, yPan;
		float xZoom, yZoom;

		wxDECLARE_EVENT_TABLE();
};

#endif // GRAPHCANVAS_H
