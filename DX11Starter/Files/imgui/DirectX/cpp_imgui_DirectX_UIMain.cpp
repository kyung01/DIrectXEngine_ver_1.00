#include "UIMain.h"
#include "imgui\imgui.h"
using namespace NImGui;

void NImGui::UIMain::init(Graphic::GraphicMain * graphicMain)
{
	this->graphicMain = graphicMain;
}

void NImGui::UIMain::render()
{
	if (!graphicMain) return; // I don't have a pointer to the instance needed to initate drawing cycle

	//ImGui::ShowTestWindow();

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		//ShowExampleMenuFile();
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		//if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		//if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
		ImGui::Separator();
		//if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		//if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		//if (ImGui::MenuItem("Paste", "CTRL+V")) {}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	ImGui::ShowTestWindow();
	ImGui::Text("hiworld");
}
