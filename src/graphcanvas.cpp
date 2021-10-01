#include "graphcanvas.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>

wxBEGIN_EVENT_TABLE(GraphCanvas, wxWindow)
	EVT_PAINT(GraphCanvas::OnPaint)
wxEND_EVENT_TABLE()

GraphCanvas::GraphCanvas(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size) : wxWindow(parent, id, pos, size){
	pan = {0.0, 0.0};
	zoom = {100.0, 100.0};
	
	oldMouse.x = -1;
	
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	Bind(wxEVT_SIZE, &GraphCanvas::OnSize, this);
	Bind(wxEVT_ERASE_BACKGROUND, &GraphCanvas::OnEraseBackground, this);
	Bind(wxEVT_MOTION, &GraphCanvas::OnMouseMove, this);
	Bind(wxEVT_MOUSEWHEEL, &GraphCanvas::OnZoom, this);
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
	size = GetSize();
	
	pen.SetColour(wxColor(255, 255, 255));
	dc.SetPen(pen);
	dc.DrawRectangle(0, 0, size.x, size.y);
	
	pen.SetColour(wxColor(0, 0, 0));
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.SetFont(font);
	
	//dc.DrawText(wxT("Hello"), wxPoint(size.x/2, size.y/2));
	
	{
		float m = 1.0f;
		vec2d start = toScreen({0.0, 0.0});
		vec2d next;
		vec2d delta;
		while(true){
			next = toScreen({1.0 * m, 1.0 * m});
			delta = next - start;
			if(delta.x < 30.0f) m *= 10.0f;
			else break;
		}
		
		dc.DrawLine(wxPoint(0, start.y), wxPoint(size.x, start.y));
		dc.DrawLine(wxPoint(start.x, 0), wxPoint(start.x, size.y));
	
		for(int i = -start.x/delta.x; i < (size.x-start.x)/delta.x; i++){
			dc.DrawText(std::to_string(i * (int)m), start + vec2d{delta.x * i, 0.0});
		}
		for(int i = (size.y-start.y)/delta.y; i < -start.y/delta.y; i++){
			dc.DrawText(std::to_string(i * (int)m), start + vec2d{0.0, delta.y * i});
		}
	}
	
	for(int i = 0; i < (int)inputs.size(); i++){
		Expression* e = expressions.at(i);
		if(e != nullptr && e->getType() == ET_OPERATION && ((Operation<2>*)e)->getOperationType() == OP_ASSIGN && ((Operation<2>*)e)->operands[0]->getType() == ET_FUNCTION){
			Function* f = (Function*)(((Operation<2>*)e)->operands[0]);
			vec2d old;
			try{
				for(int j = 0; j < size.x; j++){
					vec2d w = toWorld({(double)j, 0.0});
					
					Constant c(w.x, true);
					f->setArgument(&c);
					f->evaluate();
					numtype v = f->getValue();
					
					vec2d s = toScreen({0.0, v});
					s.x = j;
					
					if(j != 0){
						dc.DrawLine(old, s);
					}
					old = s;
				}
			}
			catch(std::exception& err){
				outputs.at(i) = err.what();
			}
		}
		
		dc.DrawText(wxString(std::string("\"") + outputs.at(i) + "\""), wxPoint(5, i*20+5));
	}
}
void GraphCanvas::OnSize(wxSizeEvent& event){
	Refresh();
	event.Skip();
}
void GraphCanvas::OnEraseBackground(wxEraseEvent& event){
	
}

void GraphCanvas::add(int i, const std::string& str){
	inputs.insert(inputs.begin()+i, str);
	expressions.insert(expressions.begin()+i, nullptr);
	outputs.insert(outputs.begin()+i, "");
	
	set(inputs.size()-1, str);
}
void GraphCanvas::set(int i, const std::string& str){
	inputs.at(i) = str;
	
	try{
		Expression* e = Expression::getExpression(str);
		e->evaluate();
		expressions.at(i) = e;
		if(e->hasValue()){
			outputs.at(i) = std::to_string(e->getValue());
		}
		else{
			outputs.at(i) = "";
			//outputs.at(i) = e->toString();
		}
	}
	catch(std::exception& err){
		outputs.at(i) = err.what();
		expressions.at(i) = nullptr;
	}
}
void GraphCanvas::remove(int i){
	inputs.erase(inputs.begin()+i);
	outputs.erase(outputs.begin()+i);
	expressions.erase(expressions.begin()+i);
}

vec2d GraphCanvas::toScreen(vec2d v){
	return vec2d{
		v.x * zoom.x + size.x/2 - pan.x,
		size.y-1 - v.y * zoom.y - size.y/2 + pan.y
	};
}
vec2d GraphCanvas::toWorld(vec2d v){
	return vec2d{
		(v.x - size.x/2 + pan.x) / zoom.x,
		(size.y-1 - v.y - size.y/2 + pan.y) / zoom.y
	};
}

void GraphCanvas::OnMouseMove(wxMouseEvent& event){
	if(event.Dragging()){
		wxPoint mousePos = event.GetPosition();
		if(oldMouse.x == -1) oldMouse = mousePos;
		
		pan.x += (oldMouse.x - mousePos.x);
		pan.y += (mousePos.y - oldMouse.y);
		
		Refresh();
		
		oldMouse = mousePos;
	}
	else{
		oldMouse.x = -1;
	}
}
void GraphCanvas::OnZoom(wxMouseEvent& event){
	double d = 1.0 + event.GetWheelRotation() / 1000.0;
	
	vec2d m = {(double)event.GetX(), (double)event.GetY()};
	vec2d p = toWorld(m);
	
	pan.x = p.x * d * zoom.x - p.x * zoom.x + pan.x;
	pan.y = p.y * d * zoom.y - p.y * zoom.y + pan.y;
	zoom *= d;
	
	Refresh();
}
