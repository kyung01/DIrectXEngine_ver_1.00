#pragma once
#include <fstream>
#include <list>
#include <vector>
#include <d3d11.h>
#include "Vertex.h"
#include <SimpleMath.h>
namespace Graphic
{
	// Contains all the information for the engineto draw this object proeprly 
	class Mesh {
	private:
		void construct(ID3D11Device *device,
			Vertex* verticies_data, int vertices_offsetEnd, int* indicies_data, int indicies_offsetEnd);
	protected:
		ID3D11Buffer *bufferVertices;
		ID3D11Buffer *bufferIndicies;
		std::list<Vertex> verticies;
		std::list<int> indices;
		void getTangent(
			Vertex & vert,
			DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector2 uv0,
			DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector2 uv1,
			DirectX::SimpleMath::Vector3 v2, DirectX::SimpleMath::Vector2 uv2);
	public:
		Mesh(ID3D11Device *device,
			Vertex* verticies_data, int vertices_offsetEnd, int* indicies_data, int indicies_offsetEnd);
		Mesh(ID3D11Device * device, char* objFile);
		~Mesh();
		
		/*
		void construct(
			ID3D11Device *device,
			Vertex* verticies_data, int vertices_offsetEnd, int* indicies_data, int indicies_offsetEnd);
		*/


		//Model(Vertex* verticies_data, int vertices_offsetEnd, int* indicies_data, int indicies_offsetEnd);
		ID3D11Buffer* getBufferVertex();
		ID3D11Buffer*&getBufferVertexRef();
		ID3D11Buffer* getBufferIndex();
		int getBufferIndexCount();

	};
}