#include "stdafx.h"
#include "ModelReader.h"


ModelReader::ModelReader(string filename)
{
	ReadModelObjData(filename);

	//expand the data suitable for lDrawArrays()
	CreateExpandedVertices();
	CreateExpandedNormals();
	CreateExpandedTextureCoordinates();
}

ModelReader::~ModelReader(void)
{
}

void ModelReader::ReadModelObjData(string filename)
{
	std::fstream modelfile(filename, std::ios_base::in);

	if(!modelfile.is_open())
	{
		std::cerr << "File " << filename << " not found.";
		DebugBreak();
		throw std::invalid_argument( "File not found" );
		return;	//ERROR!!!
	}

	string line;
	string token = "";
	while (getline(modelfile, line))
	{
		istringstream iss(line);

		// process the line
		token = "";

		iss >> token; // read to first whitespace

		if (token == "#")
		{
			// ignore rest of line
			// iss >> s;
		}
		else if (token == "g")
		{
			// read the model name
			iss >> m_modelName;
			// ignore rest of line
		}
		else if (token == "v")
		{
			ProcessVertexLine(iss);
		}
		else if (token == "vn")
		{
			ProcessVertexNormalLine(iss);
		}
		else if (token == "vt")
		{
			ProcessVertexTextureLine(iss);
		}
		else if (token == "f")
		{
			ProcessFaceLine(iss);
		}
		else
		{
			// ignore any line without these qualifiers
			// ignore rest of line
		}
	}
	modelfile.close();

}

void ModelReader::ProcessVertexLine(istringstream& iss)
{

	float vertex;

	while(iss >> vertex)
	{
		m_vertices.push_back(vertex);
	}


}

void ModelReader::ProcessVertexNormalLine(istringstream& iss)
{

	float vertexNormal;

	while(iss >> vertexNormal)
	{
		m_vertexNormals.push_back(vertexNormal);
	}
}

void ModelReader::ProcessVertexTextureLine(istringstream& iss)
{
	float textureLine;

	while(iss >> textureLine)
	{
		m_vertexTextureCoordinates.push_back(textureLine);
	}
}


void ModelReader::ProcessFaceLine(istringstream& iss)
{
	int value;

	while(iss >> value)
	{
		value--; //we need to make this a zero based index for openGL to like us
		m_faceVertexIndices.push_back(value);

		int lookAhead = iss.peek();  	// peek character
		if (lookAhead == forwardSlash)    // "/"
		{
			iss.get();
			lookAhead = iss.peek();
			if(lookAhead == forwardSlash)
			{
				iss.get();
				iss >> value;
				m_faceNormalIndices.push_back(value - 1);
			}
			else
			{
				// read the texture value
				iss >> value;
				// nd store it
				m_faceTextureIndices.push_back(value - 1);
				// this assumes we have a slash next and step over it
				iss.get();
				// read the normal value
				iss >> value;
				m_faceNormalIndices.push_back(value - 1);
			}
		}
	}
}


void ModelReader::CreateExpandedVertices()
{
	float value;

	for (std::vector<unsigned int>::iterator it = m_faceVertexIndices.begin() ; it != m_faceVertexIndices.end(); ++it)
	{
		int vertexNumber = (*it);


		int offset = vertexNumber * 3;



		value = m_vertices.at(offset);
		m_vertexTriplets.push_back(value);
		value = m_vertices.at(offset + 1);
		m_vertexTriplets.push_back(value);
		value = m_vertices.at(offset + 2);
		m_vertexTriplets.push_back(value);
	}

}
void ModelReader::CreateExpandedNormals()
{
	// we create a list of normal triplets for each face (with duplicates)
	// this will then be the same size as the vertexTriplets vector
	float value;

	for (std::vector<unsigned int>::iterator it = m_faceNormalIndices.begin() ; it != m_faceNormalIndices.end(); ++it)
	{
		//here we have a 1 base index
		// get the face number as 0 indexed
		int vertexNormalNumber = (*it);

		int offset = vertexNormalNumber * 3;

		value = m_vertexNormals.at(offset);
		m_vertexNormalTriplets.push_back(value);
		value = m_vertexNormals.at(offset + 1);
		m_vertexNormalTriplets.push_back(value);
		value = m_vertexNormals.at(offset + 2);
		m_vertexNormalTriplets.push_back(value);

	}
}
void ModelReader::CreateExpandedTextureCoordinates()
{
	// create actual vertices here (with duplicates)
	// this is in the form that glDrawArrays can work with
	//
	// assume triangles so far
	// assert((faceVertexIndices.size() % 3) == 0);

	float value;

	for (std::vector<unsigned int>::iterator it = m_faceTextureIndices.begin() ; it != m_faceTextureIndices.end(); ++it)
	{
		//here we have a 1 base index
		// get the face number as 0 indexed
		int vertexTextureNumber = (*it);

		int offset = vertexTextureNumber * 2;

		value = m_vertexTextureCoordinates.at(offset);
		m_vertexTexturePairs.push_back(value);
		value = m_vertexTextureCoordinates.at(offset + 1);
		m_vertexTexturePairs.push_back(value);

	}

}


// Get methods gove access to the vector data

vector<float>& ModelReader::GetVertices()
{
	return m_vertexTriplets;
}
vector<float>& ModelReader::GetNormals()
{
	return m_vertexNormalTriplets;
}
vector<float>& ModelReader::GetTextureCoordinates()
{
	return m_vertexTexturePairs;
}