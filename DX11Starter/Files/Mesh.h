#pragma once
#include <list>
#include "Vertex.h"
#include "DXCore.h"
namespace Graphic
{
	// Contains all the information for the engineto draw this object proeprly 
	class Mesh {
	protected:
		ID3D11Buffer *bufferVertices;
		ID3D11Buffer *bufferIndicies;
		std::list<Vertex> verticies;
		std::list<int> indices;
	public:
		Mesh(ID3D11Device *device,
			Vertex* verticies_data, int vertices_offsetEnd, int* indicies_data, int indicies_offsetEnd);
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