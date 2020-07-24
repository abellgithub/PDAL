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
}
void ObjReader::ready(PointTableRef table) {
	m_istream = Utils::openFile(m_filename, false);
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
	//TEMPORARY
	return 0;

	PointId pointId1, pointId2, pointId3;
        auto it = m_points.find(tri[0]);
        if (it != m_points.end())
        {
//            pointId1 = addPoint;
            m_points.insert({tri[0], pointId1});
        }
        else
            pointId1 = it->second;
        if (it != m_points.end())
        {
//            pointId2 = addPoint;
            m_points.insert({tri[1], pointId2});
        }
        else
            pointId1 = it->second;
        if (it != m_points.end())
        {
//            pointId3 = addPoint;
            m_points.insert({tri[2], pointId3});
        }
        else
            pointId1 = it->second;

        //... repeat for points 2 and 3 or stick in a lambda.

        m_mesh->add(pointId1, pointId2, pointId3);
    }
    return m_index;
}

bool ObjReader::newVertex(PointViewPtr view, double x, double y, double z)
{
	std::cout << "adding vertex (" << x << ", " << y << ", " << z << ")" << std::endl;
	auto point = view->point(m_index);
	m_index++;
	point.setField(Dimension::Id::X, x);
	point.setField(Dimension::Id::Y, y);
	point.setField(Dimension::Id::Z, z);
	
	return false;
}

bool ObjReader::newTextureVertex(double x, double y, double z)
{
	return false;
}

bool ObjReader::newNormalVertex(double x, double y, double z)
{
	return false;
}

bool ObjReader::newTriangle(TRI vertices)
{
	return false;
}

bool ObjReader::readFace(TRI vertices, PointViewPtr view)
{
	int debugCtr = 0;
	if(m_istream->peek() == EOF) return false;
	while(true) {
		std::string line;
		std::getline(*m_istream, line);
		Utils::trim(line);
		if(line.length() == 0) continue;
		//std::string lineType = line.substr(0, line.find(' '));
		StringList fields = Utils::split2(line, '\t');
		std::cout << "line " << debugCtr << ": " << line;
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
			// Utils::fromString(fields[1]);
			newVertex(view, 1.0, 2.0, 3.0);
			std::cout << "; vertex";
		}
		else if(Utils::startsWith(line, "vt")) {
			// Vertex texture
		}
		else if(Utils::startsWith(line,  "vn")) {
			// Vertex texture
		}
		else if(Utils::startsWith(line, "f")) {
			// Face
			// Do something...
			break;
		}
		else if(Utils::startsWith(line, "o")) {
		}
		else if(Utils::startsWith(line, "usemtl")) {
		}
		else {
			//TODO handle this case
			throwError("Unkown line type: " + line);
		}
		debugCtr++;
		std::cout << std::endl;
	}
	return true;
}

} // namespace pdal

