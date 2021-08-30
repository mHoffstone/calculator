#include "graphcanvas.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>

wxBEGIN_EVENT_TABLE(GraphCanvas, wxWindow)
	EVT_PAINT(GraphCanvas::OnPaint)
wxEND_EVENT_TABLE()

GraphCanvas::GraphCanvas(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size) : wxWindow(parent, id, pos, size){
	xPan = 0.0f;
	yPan = 0.0f;
	xZoom = 1.0f;
	yZoom = 1.0f;
	
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	Bind(wxEVT_SIZE, &GraphCanvas::OnSize, this);
	Bind(wxEVT_ERASE_BACKGROUND, &GraphCanvas::OnEraseBackground, this);
}
GraphCanvas::~GraphCanvas(){

}
void GraphCanvas::OnPaint(wxPaintEvent& event){
	Refresh();

	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);

	dc.Clear();

	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();
	pen.SetStyle(wxPENSTYLE_SOLID);
	wxSize size = GetSize();
	
	pen.SetColour(wxColor(255, 255, 255));
	dc.SetPen(pen);
	dc.DrawRectangle(0, 0, size.x, size.y);
	
	pen.SetColour(wxColor(0, 0, 0));
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.SetFont(font);
	
	dc.DrawLine(wxPoint(0, size.y/2), wxPoint(size.x, size.y/2));
	dc.DrawLine(wxPoint(size.x/2, 0), wxPoint(size.x/2, size.y));
	//dc.DrawText(wxT("Hello"), wxPoint(size.x/2, size.y/2));
	for(int i = 0; i < (int)inputs.size(); i++){
		dc.DrawText(wxString(std::string("\"") + inputs[i] + "\""), wxPoint(5, i*20+5));
	}
}
void GraphCanvas::OnSize(wxSizeEvent& event){
	Refresh();
	event.Skip();
}
void GraphCanvas::OnEraseBackground(wxEraseEvent& event){
	
}