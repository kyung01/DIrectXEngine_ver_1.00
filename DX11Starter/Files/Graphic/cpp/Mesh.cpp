#include <Graphic\Asset\Mesh.h>
#include <iostream>
using namespace NGraphic;
void Mesh::getTangent(
	Vertex & vert,
	DirectX::SimpleMath::Vector3 v0, DirectX::SimpleMath::Vector2 uv0,
	DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector2 uv1,
	DirectX::SimpleMath::Vector3 v2, DirectX::SimpleMath::Vector2 uv2)
{
	auto posDelta00 = v1 - v0;
	auto posDelta01 = v2 - v0;
	auto uvDelta00 = (uv1 - uv0);
	auto uvDelta01 = (uv2 - uv0);
	float f = 0.000001+(uvDelta01.y*uvDelta00.x - uvDelta00.y*uvDelta01.x);
	
	DirectX::SimpleMath::Vector3 A = (DirectX::SimpleMath::Vector3(0.000001) + (uvDelta01.y * posDelta00 - uvDelta00.y*posDelta01)) / f;
	DirectX::SimpleMath::Vector3 B = (DirectX::SimpleMath::Vector3(0.000001) + (uvDelta00.x * posDelta01 - uvDelta01.x * posDelta00)) / f;
	A.Normalize();
	B.Normalize();
	vert.tangent = A;
	vert.biTangent = B;

	//posDelta00 = uvDelta00.x * A + uvDelta00.y * B
	//posDelta01 = uvDelta01.x * A + uvDelta01.y * B
	//solve for A
	//(posDelta00 - uvDelta00.y * B ) / uvDelta00.x = A
	//(posDelta01 - uvDelta01.y * B ) / uvDelta01.x = A
	//(posDelta00 - uvDelta00.y * B ) / uvDelta00.x = (posDelta01 - uvDelta01.y * B ) / uvDelta01.x
	//uvDelta01.x *(posDelta00 - uvDelta00.y * B ) = uvDelta00.x * (posDelta01 - uvDelta01.y * B )
	
	//uvDelta01.x * posDelta00 - uvDelta01.x * uvDelta00.y * B
	// = uvDelta00.x * posDelta01 - uvDelta00.x *uvDelta01.y * B

	// B
	// =( + uvDelta00.x * posDelta01-uvDelta01.x * posDelta00 )
	// / (uvDelta00.x *uvDelta01.y - uvDelta01.x * uvDelta00.y )


	//posDelta00 = uvDelta00.x * A + uvDelta00.y * B
	//posDelta01 = uvDelta01.x * A + uvDelta01.y * B
	//solve for B
	//(posDelta00 - uvDelta00.x * A)/uvDelta00.y = B
	//(posDelta01 - uvDelta01.x * A)/ uvDelta01.y = B
	//uvDelta01.y * (posDelta00 - uvDelta00.x * A) = uvDelta00.y *(posDelta01 - uvDelta01.x * A)

	//uvDelta01.y * posDelta00 - uvDelta01.y*uvDelta00.x * A
	//uvDelta00.y*posDelta01 - uvDelta00.y*uvDelta01.x*A
	//A = (-uvDelta01.y * posDelta00 + uvDelta00.y*posDelta01) / (- uvDelta01.y*uvDelta00.x + uvDelta00.y*uvDelta01.x)
	//
}
Mesh::Mesh(ID3D11Device * device, char* objFile) {
	using namespace DirectX;
	// File input object
	std::ifstream obj(objFile);

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector< XMFLOAT3> positions;     // Positions from the file
	std::vector<XMFLOAT3> normals;       // Normals from the file
	std::vector<XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			getTangent(v1, v1.Position, v1.UV, v2.Position, v2.UV, v3.Position, v3.UV);
			getTangent(v2, v2.Position, v2.UV, v1.Position, v1.UV, v3.Position, v3.UV);
			getTangent(v3, v3.Position, v3.UV, v1.Position, v1.UV, v2.Position, v2.UV);

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the y
				v4.UV.y = 1.0f - v4.UV.y;

				// Add a whole triangle
				verts.push_back(v1);
				verts.push_back(v3);
				verts.push_back(v4);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
	}

	// Close the file and create the actual buffers
	obj.close();
	construct(device, &verts[0], verts.size(), (int*)&indices[0], indices.size());

	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the address of the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the address of the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
	// - Yes, the indices are a bit redundant here (one per vertex)

}

void Mesh::construct(ID3D11Device * device, Vertex * verticies_data, int vertices_offsetEnd, int * indicies_data, int indicies_offsetEnd)
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



Mesh::Mesh(ID3D11Device * device, Vertex * verticies_data, int vertices_offsetEnd, int * indicies_data, int indicies_offsetEnd)
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

Mesh::~Mesh()
{
	if (bufferVertices) { bufferVertices->Release(); }
	if (bufferIndicies) { bufferIndicies->Release(); }
}


ID3D11Buffer* Mesh::getBufferVertex()
{
	return bufferVertices;
}

ID3D11Buffer*& Mesh::getBufferVertexRef()
{
	return bufferVertices;
}

ID3D11Buffer * Mesh::getBufferIndex()
{
	return bufferIndicies;
}

int Mesh::getBufferIndexCount()
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
