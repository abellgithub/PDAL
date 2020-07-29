/******************************************************************************
* Copyright (c) 2020, Hobu Inc., info@hobu.co
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include "ObjReader.hpp"

namespace pdal
{

static StaticPluginInfo const s_info
{
    "readers.obj",
    "Obj Reader",
    "http://pdal.io/stages/readers.obj.html",
    { "obj" }
};

CREATE_STATIC_STAGE(ObjReader, s_info)

std::string ObjReader::getName() const { return s_info.name; }
void ObjReader::addArgs(ProgramArgs& args) {}
void ObjReader::addDimensions(PointLayoutPtr layout) {
	layout->registerDim(Dimension::Id::X);
	layout->registerDim(Dimension::Id::Y);
	layout->registerDim(Dimension::Id::Z);
	/*
	layout->registerDim(Dimension::Id::NormalX);
	layout->registerDim(Dimension::Id::NormalY);
	layout->registerDim(Dimension::Id::NormalZ);
	layout->registerDim(Dimension::Id::TextureX);
	layout->registerDim(Dimension::Id::TextureY);
	layout->registerDim(Dimension::Id::Red);
	layout->registerDim(Dimension::Id::Green);
	layout->registerDim(Dimension::Id::Blue);
	*/
}
void ObjReader::ready(PointTableRef table) {
	m_istream = Utils::openFile(m_filename, false);
	std::cout << "opening file: " << m_filename << std::endl;
	std::cout.flush();
	m_index = 0;
}
//point_count_t ObjReader::read(PointViewPtr view, point_count_t numPts){}
void ObjReader::done(PointTableRef table){}
point_count_t ObjReader::read(PointViewPtr view, point_count_t cnt)
{
    while (true)
    {
        TRI tri;
        bool ok = readFace(tri, view);
		// TEMPORARY
		// return 0;


		PointId pointId1, pointId2, pointId3;
		auto it = m_points.find(tri[0]);
        if (it == m_points.end())
        {
            //pointId1 = view->point((point_count_t)pointId1);;
			//PointRef pt = view->point(m_index);
			//m_index++;
			pointId1 = addPoint(view, tri[0]);
            m_points.insert({tri[0], pointId1});
        }
        else
            pointId1 = it->second;
        it = m_points.find(tri[1]);
        if (it == m_points.end())
        {
            //pointId2 = view->point(pointId2);
			//PointRef pt = view->point(m_index);
			//m_index++;
			pointId2 = addPoint(view, tri[1]);
            m_points.insert({tri[1], pointId2});
        }
        else
            pointId2 = it->second;
        it = m_points.find(tri[2]);
        if (it == m_points.end())
        {
            //pointId3 = view->point(pointId3);
			//PointRef pt = view->point(m_index);
			//m_index++;
			pointId3 = addPoint(view, tri[2]);
            m_points.insert({tri[2], pointId3});
        }
        else
            pointId3 = it->second;


        //m_mesh->add(pointId1, pointId2, pointId3);
		if(!ok) return m_index;
    }
    return m_index;
}
PointId ObjReader::addPoint(PointViewPtr view, VTN vertex) {
	XYZ v, t, n;
	PointRef pt = view->point(m_index);
	m_index++;

	int64_t vertexIndex = std::get<0>(vertex);
	v = m_vertices.at(vertexIndex);
	pt.setField(Dimension::Id::X, v.x);
	pt.setField(Dimension::Id::Y, v.y);
	pt.setField(Dimension::Id::Z, v.z);

	int64_t textureIndex = std::get<1>(vertex);
	if(textureIndex >= 0) {
		t = m_textureVertices.at(textureIndex);
		pt.setField(Dimension::Id::TextureX, t.x);
		pt.setField(Dimension::Id::TextureY, t.y);
	}

	int64_t normalIndex = std::get<2>(vertex);
	if(normalIndex >= 0) {
		n = m_normalVertices.at(normalIndex);
		pt.setField(Dimension::Id::NormalX, n.x);
		pt.setField(Dimension::Id::NormalY, n.y);
		pt.setField(Dimension::Id::NormalZ, n.z);
	}

	return pt.pointId();
}

bool ObjReader::newVertex(PointViewPtr view, double x, double y, double z)
{
	m_vertices.push_back({x, y, z});
	return false;
}

bool ObjReader::newTextureVertex(double x, double y, double z)
{
	m_textureVertices.push_back({x, y, z});
	return false;
}

bool ObjReader::newNormalVertex(double x, double y, double z)
{
	m_normalVertices.push_back({x, y, z});
	return false;
}

bool ObjReader::newTriangle(TRI vertices)
{
	// Check if 
	// TODO add to m_mesh
	return false;
}

bool ObjReader::readFace(TRI vertices, PointViewPtr view)
{
	int debugCtr = 0;
	while(true) {
		if(m_istream->peek() == EOF) return false;
		std::string line;
		std::cout.flush();
		std::getline(*m_istream, line);
		Utils::trim(line);
		if(line.length() == 0) continue;
		//std::string lineType = line.substr(0, line.find(' '));
		StringList fields = Utils::split2(line, ' ');
		if(Utils::startsWith(line, "#")) {
			// Coment
			// Do nothing
		}
		/*
		else if(line.length() == 0) {
			// Empty
			// Do nothing
		}
		*/
		else if(Utils::startsWith(line, "v")) {
			// Vertex
			double x, y, z;
			//std::cout << "fields length: " << fields.size() << std::endl;
			Utils::fromString(fields[1], x);
			Utils::fromString(fields[2], y);
			Utils::fromString(fields[3], z);
			newVertex( view, x, y, z );
			std::cout << "; vertex";
		}
		else if(Utils::startsWith(line, "vt")) {
			// Vertex texture
			double x, y, z;
			Utils::fromString(fields[1], x);
			Utils::fromString(fields[2], y);
			Utils::fromString(fields[3], z);
			newTextureVertex( x, y, z );

		}
		else if(Utils::startsWith(line,  "vn")) {
			// Vertex texture
			double x, y, z;
			Utils::fromString(fields[1], x);
			Utils::fromString(fields[2], y);
			Utils::fromString(fields[3], z);
			newNormalVertex( x, y, z );
		}
		else if(Utils::startsWith(line, "f")) {
			// Face
			// Do something...
			// PointId x, y, z;

            if (fields.size() < 4)
                throwError("Not enough vertices in face specification.");
            StringList vertices(fields.begin() + 1, fields.end());
            extractFace(vertices);
		}
		else if(Utils::startsWith(line, "o")) {
		}
		else if(Utils::startsWith(line, "usemtl")) {
		}
		else {
			//TODO handle this case
			// For now, we just ignore the line and move on
			//throwError("Unkown line type: " + line);
		}
		debugCtr++;
		std::cout << std::endl;
	}
	return true;
}

void ObjReader::extractFace(StringList fields)
{
    TRI tri;

    for (size_t i = 0; i < 3; ++i)
        tri[i] = extractVertex(fields[0]);
    newTriangle(tri);
}

ObjReader::VTN ObjReader::extractVertex(const std::string& vstring)
{
    size_t len;
    VTN vtn = { -1, -1, -1 };
    StringList parts = Utils::split(vstring, '/');

    if (parts.size() > 3)
        throwError("Too many items in vertex specification.");

    std::get<0>(vtn) = std::stoi(parts[0], &len);
    if (len != parts[0].size())
        throwError("Invalid index in face specification.");

    if (parts.size() > 1)
    {
        int i = std::stoi(parts[1], &len);
        if (len != 0 && len != parts[1].size())
            throwError("Invalid index in face specification.");
        if (i)
            std::get<1>(vtn) = i;
    }

    if (parts.size() > 2)
    {
        int i = std::stoi(parts[1], &len);
        if (len != 0 && len != parts[1].size())
            throwError("Invalid index in face specification.");
        if (i)
            std::get<2>(vtn) = i;
    }
    return vtn;
}

} // namespace pdal
