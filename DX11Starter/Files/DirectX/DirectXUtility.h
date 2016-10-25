#pragma once
#include <d3d11.h>
#include <string>
namespace DirectX {
	struct DirectXUtility {
		static std::string HRESULT_TO_STRING(HRESULT hrs) {
			std::string translated = "";
			switch (hrs) {
			case S_OK:
				translated = "S_OK"; break;
			case E_NOTIMPL:
				translated = "E_NOTIMPL"; break;
			case E_NOINTERFACE:
				translated = "E_NOINTERFACE"; break;
			case E_POINTER:
				translated = "E_POINTER"; break;
			case E_ABORT:
				translated = "E_ABORT"; break;
			case E_FAIL:
				translated = "E_FAIL"; break;
			case E_UNEXPECTED:
				translated = "E_UNEXPECTED"; break;
			case E_ACCESSDENIED:
				translated = "E_ACCESSDENIED"; break;
			case E_HANDLE:
				translated = "E_HANDLE"; break;
			case E_OUTOFMEMORY:
				translated = "E_OUTOFMEMORY"; break;
			case E_INVALIDARG:
				translated = "E_INVALIDARG"; break;
			default:
				translated = "E_UNDEFEIND_SHIT"; break;
			}
			return translated;
		};
	};
}