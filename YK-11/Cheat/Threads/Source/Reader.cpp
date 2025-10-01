#include "../Reader.h"
#include "../../Offsets/Offsets.h"
#include "../../../imgui/Menu/Overlay.h"

Cache cache[10] = {};

BasePawn array[10] = {};
BaseEntity array2[10] = {};

float pythag(Vector3 v1, Vector3 v2) 
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

// distance = (huDistance / 16) / 3.28084;  convert from hammer units to meters


void Reader()
{
	__int8 count = 0;
	unsigned __int64 lol = Attach();

	for (;;)
	{
		ZeroMemory(array, 10 * sizeof(BasePawn));
		ZeroMemory(array2, 10 * sizeof(BaseEntity));

		count = 0;
		for (int i = 0; i < 64; i++)
		{
			unsigned __int64 entityList = Read<unsigned __int64>(lol + dwEntityList);

			unsigned __int64 listEntry = Read<unsigned __int64>(entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
			if (!listEntry)
				continue;

			unsigned __int64 currentController = Read<unsigned __int64>(listEntry + (120) * (i & 0x1FF));
			if (!currentController)
				continue;

			bool isme = Read<bool>(currentController + m_bIsLocalPlayerController);
			if (isme)
				continue;

			unsigned __int64 pawnHandle = Read<unsigned __int64>(currentController + m_hPlayerPawn);
			if (!pawnHandle)
				continue;

			std::string name = Read<std::string>(currentController + m_iszPlayerName);

			unsigned __int64 listEntry2 = Read<unsigned __int64>(entityList + (0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16));
			if (!listEntry2)
				continue;
			unsigned __int64 currentPawn = Read<unsigned __int64>(listEntry + (120) * (pawnHandle & 0x1FF));
			if (!currentPawn)
				continue;

			Vector3 camerapos = Read<Vector3>(currentPawn + m_vecLastClipCameraPos);

			BaseEntity be = Read<BaseEntity>(currentPawn);
			array2[count] = be;

			BasePawn bp = Read<BasePawn>(currentPawn);
			array[count] = bp;

			view_matrix_t vm = Read<view_matrix_t>(lol + dwViewMatrix);


			cache[count] = {
				vm,
				bp.m_vOldOrigin,
				camerapos,
				be.m_iHealth,
				be.m_iTeamNum,
				name
			};

			count++;
		}
	}
}


void RenderOverlayThread() 
{
	Vector2 cam = {};
	Vector2 cam_pos = {};
	overlay.CreateOverlay();
	overlay.CreateDevice();
	overlay.CreateImGui();


	overlay.SetForeground(GetConsoleWindow());

	for (;;)
	{
		overlay.StartRender();

		if (esp)
		{
			for (int i = 0; i < _countof(cache); i++)
			{
				if (W2S(cache[i].Position, cam, cache[i].vm))
				{
					if (!cache[i].health)
						continue;
					W2S(cache[i].CameraPos, cam_pos, cache[i].vm);
					ImVec2 top = ImVec2(cam_pos.x, cam_pos.y);
					ImVec2 bottom = ImVec2(cam.x, cam.y);

					float height = bottom.y - top.y;
					float width = height / 2.0f; // typical ESP ratio

					if (cache[i].team == 2)
					{
						ImGui::GetBackgroundDrawList()->AddRect(
							ImVec2(top.x - width / 2.0f, top.y),
							ImVec2(top.x + width / 2.0f, bottom.y),
							IM_COL32(255, 255, 255, 255),
							0.0f, 0, 1.0f
						);

						int health = cache[i].health; 

						char buffer[32];
						snprintf(buffer, sizeof(buffer), "%d", health);

						ImGui::GetBackgroundDrawList()->AddText(
							ImVec2(cam_pos.x - 7.0f, cam_pos.y - 15.0f),
							IM_COL32(255, 255, 255, 255),
							buffer
						);
						ImGui::GetBackgroundDrawList()->AddText(
							ImVec2(cam.x - 7.0f, cam.y + 2.0f),
							IM_COL32(255, 255, 0, 255),
							cache[i].name.c_str()
						);
					}

					if (cache[i].team == 3)
					{
						ImGui::GetBackgroundDrawList()->AddRect(
							ImVec2(top.x - width / 2.0f, top.y),
							ImVec2(top.x + width / 2.0f, bottom.y),
							IM_COL32(0, 0, 255, 255),
							0.0f, 0, 1.0f
						);

						int health = cache[i].health;

						char buffer[32];
						snprintf(buffer, sizeof(buffer), "%d", health);

						ImGui::GetBackgroundDrawList()->AddText(
							ImVec2(cam_pos.x - 7.0f, cam_pos.y - 15.0f),
							IM_COL32(255, 255, 255, 255),
							buffer
						);

						ImGui::GetBackgroundDrawList()->AddText(
							ImVec2(cam.x - 7.0f, cam.y + 2.0f),
							IM_COL32(255, 0, 255, 255),
							cache[i].name.c_str()
						);
					}

				}
			}
		}

		if (overlay.RenderMenu) 
		{
			overlay.Render();
		}

		else 
		{
			ImGui::GetBackgroundDrawList()->AddText({ 0, 0 }, ImColor(0.0f, 1.0f, 0.0f), "YK-11 PRESS INSERT TO SHOW");
		}

		overlay.EndRender();
	}

	overlay.DestroyImGui();
	overlay.DestroyDevice();
	overlay.DestroyOverlay();
}


