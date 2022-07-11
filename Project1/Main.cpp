#include "Main.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "Okno")
{
	if (std::filesystem::exists("save.txt"))
	{
		FILE* file = fopen("save.txt", "r");
		while (!feof(file))
		{
			int key;
			int val;
			fscanf(file, "%i %i", &key, &val);
			mp[key] = val;
		}
		fclose(file);
	}

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

	input = new wxTextCtrl(this, wxID_ANY, "Wpisz port", wxPoint(10, 50), wxSize(90, 40));
	sizer2->Add(input, 1, wxEXPAND);

	button = new wxButton(this, 1000, "Start", wxPoint(10, 90), wxSize(90, 40));
	sizer2->Add(button, 2, wxEXPAND);

	button2 = new wxButton(this, 2000, "Meta", wxPoint(10, 130), wxSize(90, 40));
	button2->Enable(false);
	sizer2->Add(button2, 2, wxEXPAND);

	button3 = new wxButton(this, 3000, "Koniec", wxPoint(10, 130), wxSize(90, 40));
	button3->Enable(false);
	sizer2->Add(button3, 2, wxEXPAND);

	button4 = new wxButton(this, 4000, "Zapisz wyniki", wxPoint(10, 130), wxSize(90, 40));
	button4->Enable(false);
	sizer2->Add(button4, 2, wxEXPAND);

	sizer->Add(sizer2, 1, wxEXPAND);

	text = new wxStaticText(this, wxID_ANY, "");
	sizer->Add(text, 2, wxEXPAND);

	this->SetSizerAndFit(sizer);
}

void Main::begin(wxCommandEvent& evt)
{
	flag = false;
	flag2 = false;
	button->Enable(false);
	button2->Enable(true);
	button4->Enable(false);
	vec.clear();
	res.clear();
	text->SetLabel("");
	std::thread th(&Main::handle, this);
	th.detach();
}

void Main::handle()
{
	SimpleSerial serial(std::string(input->GetValue()), 9600);
	do
	{
		int x = serial.readLine();
		if (res.find(x) != res.end())	continue;
		if (flag2)	res.emplace(x);
		vec.push_back(x);
		text->SetLabel(text->GetLabel() + " " + std::to_string(x));
		text->Wrap(180);
	} while (!flag);
}

void Main::mark(wxCommandEvent& evt)
{
	flag2 = true;
	text->SetLabel(text->GetLabel() + "|");
	button2->Enable(false);
	button3->Enable(true);
}

void Main::end(wxCommandEvent& evt)
{
	flag = true;
	button3->Enable(false);
	button->Enable(true);
	button4->Enable(true);
}

void Main::handle_result(wxCommandEvent& evt)
{
	wxDirDialog* dir = new wxDirDialog(this, "Wybierz folder");
	dir->SetPath("");
	dir->ShowModal();

	wxTextEntryDialog* name = new wxTextEntryDialog(this, "Wpisz nazwê pliku");
	name->SetValue("");
	name->ShowModal();

	if (dir->GetPath() == "" || name->GetValue() == "")	return;

	wxString path = dir->GetPath() + "/" + name->GetValue();

	wxFile* file = new wxFile();
	file->Create(path);

	int max = 0;

	std::unordered_map<int, int> map;

	for (auto& x : res)
	{
		map[x] = std::count(vec.begin(), vec.end(), x);
		max = map[x] > max ? map[x] : max;
	}

	for (auto& [key, val] : map)	file->Write(std::to_string(key) + ": " + std::to_string(val) + '\n');

	int place = 1;

	while (max > 0)
	{
		for (auto& x : res)
		{
			if (map[x] == max)
			{
				file->Write("\n" + std::string("Miejsce ") + std::to_string(place) + ". " + std::to_string(x) + " (" + std::to_string(5 - (place - 1)) + " pkt)");
				mp[x] += (5 - (place - 1));
				place++;
			}
		}
		max--;
	}
	file->Write("\n");

	FILE* f = fopen("save.txt", "w");

	for (auto& [key, val] : mp)
	{
		file->Write("\n" + std::string("Za³oga ") + std::to_string(key) + ": " + std::to_string(val) + " pkt");
		fprintf(f, "%i %i ", key, val);
	}
	fclose(f);
}

BEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_BUTTON(1000, begin)
	EVT_BUTTON(2000, mark)
	EVT_BUTTON(3000, end)
	EVT_BUTTON(4000, handle_result)
END_EVENT_TABLE()