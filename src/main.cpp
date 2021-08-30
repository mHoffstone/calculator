#include <wx/wx.h>
#include <wx/splitter.h>

#include <iostream>
#include <vector>

#include "graphcanvas.h"

class MainApp : public wxApp{
	public:
		virtual bool OnInit();
};

class ScrollPane : public wxScrolledWindow{
	public:
		ScrollPane(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id){
			sizer = new wxBoxSizer(wxVERTICAL);
			this->SetSizer(sizer);
			this->SetScrollRate(5, 5);
		}
		
		wxBoxSizer* sizer;
};

class MainFrame : public wxFrame{
	public:
		MainFrame();

	private:
		const wxFont font = wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		void addTextController();
		void removeTextController(int index);

		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnText(wxCommandEvent& event);

		wxSplitterWindow* splitter;
		ScrollPane* scrollPane;
		GraphCanvas* canvas;

		std::vector<wxTextCtrl*> textControllers;
		std::vector<wxTextCtrl*> deletePool;
};

enum ID{
	HELLO = 1
};

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit(){
	wxLog* logger = new wxLogStream(&std::cout);
	wxLog::SetActiveTarget(logger);

	MainFrame *frame = new MainFrame();
	frame->Show(true);
	return true;
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Calculator", wxDefaultPosition){
	/*wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID::HELLO, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");*/

	SetThemeEnabled(true);
	
	splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
	scrollPane = new ScrollPane(splitter, wxID_ANY);
	canvas = new GraphCanvas(splitter, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
	
	addTextController();
	
	splitter->SetMinimumPaneSize(200);
	splitter->SplitVertically(scrollPane, canvas);
	
	Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID::HELLO);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_TEXT, &MainFrame::OnText, this);
}

void MainFrame::addTextController(){
	wxTextCtrl* text = new wxTextCtrl(scrollPane, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	text->SetFont(font);
	textControllers.push_back(text);
	scrollPane->sizer->Insert(textControllers.size()-1,
		text,
		0,							// make vertically unstretchable
		wxEXPAND |					// make horizontally stretchable
		wxLEFT | wxTOP | wxRIGHT,	// and make border top, left and right
		5							// border size
	);

	scrollPane->FitInside();
	scrollPane->sizer->Layout();

	canvas->inputs.push_back("");
}

void MainFrame::removeTextController(int index){
	wxTextCtrl* txtCtrl = textControllers[index];
	deletePool.push_back(txtCtrl);
	textControllers.erase(textControllers.begin()+index);
	canvas->inputs.erase(canvas->inputs.begin()+index);
	scrollPane->sizer->Detach(index);
	txtCtrl->Hide();
	scrollPane->FitInside();
	scrollPane->sizer->Layout();
}

void MainFrame::OnExit(wxCommandEvent& event){
	Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event){
	wxMessageBox("This is a graphing calculator.", "About", wxOK | wxICON_INFORMATION);
}
void MainFrame::OnHello(wxCommandEvent& event){
	wxLogMessage("Hello world from wxWidgets!");
}
void MainFrame::OnText(wxCommandEvent& event){
	while(deletePool.size() > 0){
		wxTextCtrl* txtCtrl = deletePool[0];
		deletePool.erase(deletePool.begin());
		txtCtrl->Destroy();
	}
	for(int i = 0; i < (int)textControllers.size(); i++){
		wxTextCtrl* txtCtrl = textControllers[i];
		std::string text = txtCtrl->GetLineText(0).ToStdString();
		if(text.empty() && i != (int)textControllers.size()-1){
			removeTextController(i);
			i--;
		}
		else{
			canvas->inputs[i] = text;
		}
	}
	if(!textControllers[(int)textControllers.size()-1]->GetLineText(0).ToStdString().empty()){
		addTextController();
	}
	canvas->Refresh();
}
