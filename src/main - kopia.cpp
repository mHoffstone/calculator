#include <wx/wx.h>

#include <iostream>
#include <vector>

#include "graphcanvas.h"

class MainApp : public wxApp{
	public:
		virtual bool OnInit();
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
		void OnSize(wxSizeEvent& event);
		void OnText(wxCommandEvent& event);
		void OnMouse(wxMouseEvent& event);

		GraphCanvas* canvas;
		wxScrolledWindow* scroll;
		wxBoxSizer* scrollsizer;
		wxBoxSizer* inputsizer;
		wxBoxSizer* topsizer;

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

	topsizer = new wxBoxSizer(wxHORIZONTAL);
	
	scrollsizer = new wxBoxSizer(wxVERTICAL);
	scrollsizer->SetMinSize(wxSize(300, -1));
	scroll = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	scroll->SetMinSize(wxSize(300, -1));
	//scroll->SetScrollbars(wxVERTICAL, 0, 16, 50);
	
	inputsizer = new wxBoxSizer(wxVERTICAL);
	scroll->SetSizer(inputsizer);
	scrollsizer->Add(scroll, 0, wxALL, 5, nullptr);
	
	canvas = new GraphCanvas(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));

	addTextController();
	
	topsizer->Add(scrollsizer, 0, wxALL);
	topsizer->Add(canvas,
		1,            // make vertically stretchable
		wxEXPAND |    // make horizontally stretchable
		wxALL        // and make border all around
	);
	
	//Layout();
	
	SetSizerAndFit(topsizer);

	Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID::HELLO);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
	Bind(wxEVT_TEXT, &MainFrame::OnText, this);
	
	scroll->Bind(wxEVT_MOTION, &MainFrame::OnMouse, this);
}

void MainFrame::addTextController(){
	//wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
	wxTextCtrl* text = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	text->SetFont(font);
	text->Fit();
	textControllers.push_back(text);
	inputsizer->Insert(textControllers.size()-1,
		text,
		0,            	// make vertically unstretchable
		wxLEFT | wxTOP,	// and make border top and left
		10				// border size
	);
	// Resize sizer or scroll
	//inputsizer->Fit();
	//scroll->Fit();
	topsizer->Layout();

	canvas->inputs.push_back("");
}

void MainFrame::removeTextController(int index){
	wxTextCtrl* txtCtrl = textControllers[index];
	deletePool.push_back(txtCtrl);
	textControllers.erase(textControllers.begin()+index);
	canvas->inputs.erase(canvas->inputs.begin()+index);
	inputsizer->Detach(index);
	txtCtrl->Hide();
	topsizer->Layout();
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
void MainFrame::OnSize(wxSizeEvent& event){
	canvas->Refresh();
	event.Skip();
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
void MainFrame::OnMouse(wxMouseEvent& event){
	std::cout << "Event" << std::endl;
	event.Skip();
}
