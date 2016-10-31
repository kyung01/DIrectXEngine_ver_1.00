#include "imgui\DirectX\UIMain.h"
#include "imgui\imgui.h"
using namespace NImGui;



void NImGui::UIMain::init(Graphic::GraphicMain * graphicMain)
{
	this->graphicMain = graphicMain;
}
void NImGui::UIMain::render(std::map<Graphic::RENDER_TYPE, Graphic::RenderTexture*> renderTexutres)
{
	ImGui::Begin("RenderTextures Demo", 0, ImGuiWindowFlags_ShowBorders);

	for (auto it = renderTexutres.begin(); it != renderTexutres.end(); it++) {
		ImGui::Text("RenderTextures", 500, 500);
		ImTextureID tex_id = it->second->m_shaderResourceView;
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		ImGui::Text("%.0fx%.0f", 500, 500);
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	}
	ImGui::End();
}
void NImGui::UIMain::render(std::map<int, Graphic::DepthTexture*> textures)
{
	ImGui::Begin("DepthTextures Demo", 0, ImGuiWindowFlags_ShowBorders);

	for (auto it = textures.begin(); it != textures.end(); it++) {
		ImGui::Text(""+it->first, 500, 500);
		ImTextureID tex_id = it->second->m_shaderResourceView;
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		//ImGui::Text("%.0fx%.0f", 500, 500);
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

	}
	ImGui::End();
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
		ImGui::Separator();
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	{
		ImGui::Begin("JESUS I HATE THIS Demo", 0, ImGuiWindowFlags_ShowBorders);
		ImTextureID tex_id = graphicMain->m_depth.m_shaderResourceView;
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		ImGui::Text("%.0fx%.0f", 500, 500);
		ImGui::Image(tex_id, ImVec2(500, 500), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

		ImGui::End();
	}

	render(graphicMain->m_renderTextures);
	render(graphicMain->m_lightDepthTextures);

	ImGui::Begin("Other window", 0, ImGuiWindowFlags_ShowBorders);
	
	if (ImGui::TreeNode("Images"))
	{
		ImGui::TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!");
		ImVec2 tex_screen_pos = ImGui::GetCursorScreenPos();
		float tex_w = 100;
		float tex_h = 100;
		ImTextureID tex_id = graphicMain->m_renderTextures.begin()->second->m_shaderResourceView;
		//ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
		ImGui::Text("%.0fx%.0f", tex_w, tex_h);
		ImGui::Image(tex_id, ImVec2(tex_w, tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			float focus_sz = 32.0f;
			float focus_x = ImGui::GetMousePos().x - tex_screen_pos.x - focus_sz * 0.5f; if (focus_x < 0.0f) focus_x = 0.0f; else if (focus_x > tex_w - focus_sz) focus_x = tex_w - focus_sz;
			float focus_y = ImGui::GetMousePos().y - tex_screen_pos.y - focus_sz * 0.5f; if (focus_y < 0.0f) focus_y = 0.0f; else if (focus_y > tex_h - focus_sz) focus_y = tex_h - focus_sz;
			ImGui::Text("Min: (%.2f, %.2f)", focus_x, focus_y);
			ImGui::Text("Max: (%.2f, %.2f)", focus_x + focus_sz, focus_y + focus_sz);
			ImVec2 uv0 = ImVec2((focus_x) / tex_w, (focus_y) / tex_h);
			ImVec2 uv1 = ImVec2((focus_x + focus_sz) / tex_w, (focus_y + focus_sz) / tex_h);
			ImGui::Image(tex_id, ImVec2(128, 128), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			ImGui::EndTooltip();
		}
		ImGui::TextWrapped("And now some textured buttons..");
		static int pressed_count = 0;
		for (int i = 0; i < 8; i++)
		{
			ImGui::PushID(i);
			int frame_padding = -1 + i;     // -1 = uses default padding
			if (ImGui::ImageButton(tex_id, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f / tex_w, 32 / tex_h), frame_padding, ImColor(0, 0, 0, 255)))
				pressed_count += 1;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::NewLine();
		ImGui::Text("Pressed %d times.", pressed_count);
		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::ShowTestWindow();
	ImGui::Text("hiworld");
}
