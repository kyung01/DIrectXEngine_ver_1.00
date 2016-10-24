#include "Mesh.h"


Graphic::Mesh::Mesh(ID3D11Device * device, Vertex * verticies_data, int vertices_offsetEnd, int * indicies_data, int indicies_offsetEnd)
{

	verticies.insert(verticies.begin(), verticies_data, verticies_data + vertices_offsetEnd);
	indices.insert(indices.begin(), indicies_data, indicies_data + indicies_offsetEnd);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) *vertices_offsetEnd;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indicies_offsetEnd;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = verticies_data;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indicies_data;

	device->CreateBuffer(&vbd, &initialVertexData, &this->bufferVertices);
	device->CreateBuffer(&ibd, &initialIndexData, &this->bufferIndicies);
}

Graphic::Mesh::~Mesh()
{
	if (bufferVertices) { bufferVertices->Release(); }
	if (bufferIndicies) { bufferIndicies->Release(); }
}


ID3D11Buffer* Graphic::Mesh::getBufferVertex()
{
	return bufferVertices;
}

ID3D11Buffer*& Graphic::Mesh::getBufferVertexRef()
{
	return bufferVertices;
}

ID3D11Buffer * Graphic::Mesh::getBufferIndex()
{
	return bufferIndicies;
}

int Graphic::Mesh::getBufferIndexCount()
{
	return indices.size();
}


/*
Graphic::Model::Model()
{
}

Graphic::Model::Model(Vertex * verticies_data, int vertices_offsetEnd, int * indicies_data, int indicies_offsetEnd)
{
	//this->construct(verticies_data, vertices_offsetEnd, indicies_data, indicies_offsetEnd);
}
*/
