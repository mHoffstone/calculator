#include "graphcanvas.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>

#include <sstream>
#include <iomanip>

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

std::string toString(double d, int p){
	std::ostringstream stream;
	stream << std::fixed;
	stream << std::setprecision(p);
	stream << d;
	return stream.str();
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
		double m = std::pow(10.0, std::floor(std::log10(300.0/zoom.x)));
		vec2d start = toScreen({0.0, 0.0});
		vec2d next = toScreen({1.0 * m, 1.0 * m});
		vec2d delta = next - start;
		
		dc.DrawLine(wxPoint(0, start.y), wxPoint(size.x, start.y));
		dc.DrawLine(wxPoint(start.x, 0), wxPoint(start.x, size.y));
		
		int p = -std::log10(m);
		if(p < 0) p = 0;
	
		for(int i = -start.x/delta.x; i < (size.x-start.x)/delta.x; i++){
			dc.DrawText(toString(i * m, p), start + vec2d{delta.x * i, 0.0});
		}
		for(int i = (size.y-start.y)/delta.y; i < -start.y/delta.y; i++){
			dc.DrawText(toString(i * m, p), start + vec2d{0.0, delta.y * i});
		}
	}
	
	int onlyDrawFunctionIndex = 0;
	for(int i = 0; i < (int)inputs.size(); i++){
		Expression* e = expressions.at(i);
		if(e != nullptr && e->getType() == ET_OPERATION && ((Operation<2>*)e)->getOperationType() == OP_ASSIGN){
			if(((Operation<2>*)e)->operands[0]->getType() == ET_FUNCTION){
				Function* f = (Function*)(((Operation<2>*)e)->operands[0]);
				bool isDrawOnly = false;
				std::pair<Expression*, FunctionArgument*> p;
				if(f->getName() == "y"){
					isDrawOnly = true;
					p = Expression::getDrawFunction(onlyDrawFunctionIndex);
					onlyDrawFunctionIndex++;
				}
				vec2d old;
				try{
					for(int j = 0; j < size.x; j++){
						vec2d w = toWorld({(double)j, 0.0});
						
						Constant c(w.x, true);
						numtype v;
						if(!isDrawOnly){
							f->setArgument(&c);
							f->evaluate(false);
							v = f->getValue();
						}
						else{
							// Set argument for function and evaluate for draw only
							p.second->e = &c;
							p.second->e->evaluate(false);
							p.first->evaluate(false);
							v = p.first->getValue();
						}
						
						vec2d s = toScreen({0.0, v});
						s.x = j;
						
						if(j != 0){
							dc.DrawLine(old, s);
						}
						old = s;
					}
				}
				/*catch(expression_exception& err){
					if(err.get_type() == expression_exception::T_NO_VALUE){
						outputs.at(i) = e->toString();
					}
					else{
						outputs.at(i) = err.what();
					}
				}*/
				catch(std::exception& err){
					outputs.at(i) = err.what();
				}
			}
			else if(((Operation<2>*)e)->operands[0]->getType() == ET_VARIABLE){
				try{
					((Operation<2>*)e)->operands[0]->evaluate(false);
					outputs.at(i) = Expression::toString(((Operation<2>*)e)->operands[0]->getValue());
				}
				catch(const expression_exception& ex){
					if(ex.get_type() == expression_exception::T_NO_VALUE){
						outputs.at(i) = ((Operation<2>*)e)->operands[0]->toString();
					}
					else outputs.at(i) = ex.what();
				}
				catch(const std::exception& err){
					outputs.at(i) = err.what();
				}
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
	
	set(i, str);
}
void GraphCanvas::set(int i, const std::string& str){
	inputs.at(i) = str;
	
	Expression* e = nullptr;
	expressions.at(i) = nullptr;
	try{
		e = Expression::getExpression(str);
		e->evaluate(false);
		expressions.at(i) = e;
		/*if(e->hasValue()){
			outputs.at(i) = Expression::toString(e->getValue());
		}
		else{*/
			//outputs.at(i) = "";
			outputs.at(i) = e->toString();
		//}
	}
	catch(const expression_exception& err){
		if(err.get_type() == expression_exception::T_NO_VALUE){
			if(e == nullptr){
				outputs.at(i) = "???";
			}
			else{
				outputs.at(i) = e->toString();
				//outputs.at(i) = err.what();
			}
		}
		else{
			outputs.at(i) = err.what();
		}
	}
	catch(const std::exception& err){
		outputs.at(i) = std::string("Other error: ") + err.what();
		//outputs.at(i) = err.what();
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
