#include "config.h"
#include "halfedgemesh.h"
#include <chrono>
#include <cstring>
#include <iostream>
using namespace Math;

namespace HalfEdges
{
	HalfEdgeMesh::HalfEdgeMesh()
	{
		
	}
	
	HalfEdgeMesh::HalfEdgeMesh(MeshResources* mesh)
	{
		this->mesh = mesh;
	}

	HalfEdgeMesh::~HalfEdgeMesh()
	{
		
	}
	
	void HalfEdgeMesh::ConstructHalfEdgeMesh()
	{
		for(int i = 0; i < mesh->GetMesh().Size(); i++)
		{
			Math::Vertex meshVert = mesh->GetMesh()[i];
			HalfEdges::Vertex* vert = new HalfEdges::Vertex();
			vert->pos = meshVert.pos;
			vert->uv = meshVert.uv;
			vert->norm = meshVert.norm;
			vert->edge = nullptr;
			vertices.Append(vert);
			vert->idx = vertices.Size()-1;
		}
		
		for(int i = 0; i < mesh->GetMeshFaces().Size(); ++i)
		{
			faces.Append(new HalfEdges::Face());
			HalfEdges::Face* face = faces[faces.Size()-1];
			for(int j = 0; j < mesh->GetMeshFaces()[i].verts.size(); ++j)
			{
				HalfEdges::HalfEdge* lastEdge;
				HalfEdges::HalfEdge* firstEdge;
				
				if(j == mesh->GetMeshFaces()[i].verts.size()-1)
				{
					halfedges.Append(new HalfEdges::HalfEdge());
					HalfEdges::HalfEdge* edge = halfedges[halfedges.Size()-1];
					edge->vert = vertices[mesh->GetMeshFaces()[i].verts[j]];
					edge->nextEdge = firstEdge;
					edge->oppositeEdge = nullptr;
					
					std::pair<unsigned int, unsigned int> edgePair(mesh->GetMeshFaces()[i].verts[0],mesh->GetMeshFaces()[i].verts[j]);
					if(edges.find(edgePair) == edges.end())
					{
						edgePair.first = mesh->GetMeshFaces()[i].verts[j];
						edgePair.second = mesh->GetMeshFaces()[i].verts[0];
						edges[edgePair] = edge;
					}
					else
					{
						edges[edgePair]->oppositeEdge = edge;
						edge->oppositeEdge = edges[edgePair];
					}
					
					edge->vert->edge = edge;
					edge->face = face;
					lastEdge->nextEdge = edge;
					
				}
				else if(j == 0)
				{
					halfedges.Append(new HalfEdges::HalfEdge());
					HalfEdges::HalfEdge* edge = halfedges[halfedges.Size()-1];
					edge->vert = vertices[mesh->GetMeshFaces()[i].verts[j]];
					edge->nextEdge = nullptr;
					edge->face = face;
					edge->oppositeEdge = nullptr;
					edge->vert->edge = edge;
					firstEdge = edge;
					lastEdge = edge;
					face->faceEdge = edge;
					std::pair<unsigned int, unsigned int> edgePair(mesh->GetMeshFaces()[i].verts[j+1],mesh->GetMeshFaces()[i].verts[j]);
					if(edges.find(edgePair) == edges.end())
					{
						edgePair.first = mesh->GetMeshFaces()[i].verts[j];
						edgePair.second = mesh->GetMeshFaces()[i].verts[j+1];
						edges[edgePair] = edge;
					}
					else
					{
						edges[edgePair]->oppositeEdge = edge;
						edge->oppositeEdge = edges[edgePair];
					}
					
				}
				else
				{
					halfedges.Append(new HalfEdges::HalfEdge());
					HalfEdges::HalfEdge* edge = halfedges[halfedges.Size()-1];
					edge->vert = vertices[mesh->GetMeshFaces()[i].verts[j]];
					edge->nextEdge = nullptr;
					edge->oppositeEdge = nullptr;
					edge->face = face;
					edge->vert->edge = edge;
					lastEdge->nextEdge = edge;
					lastEdge = edge;
					std::pair<unsigned int, unsigned int> edgePair(mesh->GetMeshFaces()[i].verts[j+1],mesh->GetMeshFaces()[i].verts[j]);
					if(edges.find(edgePair) == edges.end())
					{
						edgePair.first = mesh->GetMeshFaces()[i].verts[j];
						edgePair.second = mesh->GetMeshFaces()[i].verts[j+1];
						edges[edgePair] = edge;
					}
					else
					{
						edges[edgePair]->oppositeEdge = edge;
						edge->oppositeEdge = edges[edgePair];
					}
				}
				
			}
		}
		std::cout << "Done with HalfEdge" << std::endl;
	}
	
	MeshResources* HalfEdgeMesh::HalfEdgeMeshToMesh()
	{
		
		for(int i = 0; i < vertices.Size(); ++i)
		{
			HalfEdges::Vertex* vert = vertices[i];
			heMeshIndices[(uintptr_t) vert] = i;
			Math::Vertex bufVert;
			//std::memcpy(&bufVert, vert, sizeof(HalfEdges::Vertex)-sizeof(uintptr_t));
			bufVert.pos = vert->pos;
			bufVert.uv = vert->uv;
			bufVert.norm = vert->norm;
			
			halfEdgeToMesh.mesh.Append(bufVert);
		}
		
		for(int i = 0; i < faces.Size(); ++i)
		{
			HalfEdges::HalfEdge* edge = faces[i]->faceEdge;
			halfEdgeToMesh.indices.Append(heMeshIndices[(uintptr_t)edge->vert]);
			halfEdgeToMesh.indices.Append(heMeshIndices[(uintptr_t)edge->nextEdge->vert]);
			halfEdgeToMesh.indices.Append(heMeshIndices[(uintptr_t)edge->nextEdge->nextEdge->vert]);
			if(edge->nextEdge->nextEdge->nextEdge->nextEdge == edge)
			{
				edge = edge->nextEdge->nextEdge;
				halfEdgeToMesh.indices.Append(heMeshIndices[(uintptr_t)edge->vert]);
				halfEdgeToMesh.indices.Append(heMeshIndices[(uintptr_t)edge->nextEdge->vert]);
				halfEdgeToMesh.indices.Append(heMeshIndices[(uintptr_t)edge->nextEdge->nextEdge->vert]);
			}
		}
		
		tempMesh.mesh = halfEdgeToMesh.mesh;
		tempMesh.indices = halfEdgeToMesh.indices;
		
		return &tempMesh;
	}
	
	void HalfEdgeMesh::Subdivide(int iterations)
	{
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		for(int iterationCount = 0; iterationCount < iterations; ++iterationCount)
		{
			std::chrono::high_resolution_clock::time_point before = std::chrono::high_resolution_clock::now();
			std::vector<vec3> original_pos;
			int vertSize = vertices.Size();
			for(int i = 0; i < vertSize; ++i)
			{
				original_pos.push_back(vertices[i]->pos);
			}
			
			int facesSize = faces.Size();
			for(int i = 0; i < facesSize; ++i)
			{
				Face* face = faces[i];
				HalfEdges::Face* northWest = face;
				HalfEdges::Face* northEast = new HalfEdges::Face();
				HalfEdges::Face* southWest = new HalfEdges::Face();
				HalfEdges::Face* southEast = new HalfEdges::Face();
				faces.Append(northEast);
				faces.Append(southWest);
				faces.Append(southEast);
				
				HalfEdges::Vertex* facePoint = new HalfEdges::Vertex();
				vertices.Append(facePoint);
				vec3 vecPos = (face->faceEdge->vert->pos + face->faceEdge->nextEdge->vert->pos + face->faceEdge->nextEdge->nextEdge->vert->pos + face->faceEdge->nextEdge->nextEdge->nextEdge->vert->pos)*0.25f;
				vec2 vecUV = (face->faceEdge->vert->uv + face->faceEdge->nextEdge->vert->uv + face->faceEdge->nextEdge->nextEdge->vert->uv + face->faceEdge->nextEdge->nextEdge->nextEdge->vert->uv)*0.25f;
				vec3 vecNorm = (face->faceEdge->vert->norm + face->faceEdge->nextEdge->vert->norm + face->faceEdge->nextEdge->nextEdge->vert->norm + face->faceEdge->nextEdge->nextEdge->nextEdge->vert->norm)*0.25f;
				facePoint->pos = vecPos;
				facePoint->uv = vecUV;
				facePoint->norm = vec3::Normalize(vecNorm);
				facePoint->idx = vertices.Size()-1;
				
				
				
				HalfEdges::HalfEdge* edgeTopLeft = face->faceEdge;
				HalfEdges::HalfEdge* edgeTopRight = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeLeftBot = face->faceEdge->nextEdge->nextEdge->nextEdge;
				HalfEdges::HalfEdge* edgeLeftTop = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeBotRight = face->faceEdge->nextEdge->nextEdge;
				HalfEdges::HalfEdge* edgeBotLeft = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeRightTop = face->faceEdge->nextEdge;
				HalfEdges::HalfEdge* edgeRightBot = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeTopMid = new HalfEdges::HalfEdge();
				HalfEdges::HalfEdge* edgeMidTop = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeRightMid = new HalfEdges::HalfEdge();
				HalfEdges::HalfEdge* edgeMidRight = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeMidBot = new HalfEdges::HalfEdge();
				HalfEdges::HalfEdge* edgeBotMid = new HalfEdges::HalfEdge();
				
				HalfEdges::HalfEdge* edgeMidLeft = new HalfEdges::HalfEdge();
				HalfEdges::HalfEdge* edgeLeftMid = new HalfEdges::HalfEdge();
				
				HalfEdges::Vertex* edgePointTop;
				HalfEdges::Vertex* edgePointRight;
				HalfEdges::Vertex* edgePointBot;
				HalfEdges::Vertex* edgePointLeft;
				
				facePoint->edge = edgeMidTop;
				
				
				if(edgeTopLeft->oppositeEdge->nextEdge->oppositeEdge->nextEdge->oppositeEdge == nullptr)
				{
					edgePointTop = edgeTopLeft->oppositeEdge->nextEdge->oppositeEdge->nextEdge->vert;
					edgePointTop->pos = edgePointTop->pos+(facePoint->pos *0.25f);
					edgePointTop->uv = edgePointTop->uv+(facePoint->uv*0.25f);
					edgePointTop->norm = vec3::Normalize((edgePointTop->norm + (facePoint->norm *0.25f)));
					edgeTopRight->oppositeEdge = edgeTopLeft->oppositeEdge;
					edgeTopLeft->oppositeEdge->oppositeEdge = edgeTopRight;
					edgeTopLeft->oppositeEdge = edgeTopRight->oppositeEdge->nextEdge->oppositeEdge->nextEdge;
					edgeTopLeft->oppositeEdge->oppositeEdge = edgeTopLeft;
				}
				else
				{
					edgePointTop = new HalfEdges::Vertex();
					vertices.Append(edgePointTop);
					edgePointTop->edge = edgeTopRight;
					vec3 vecPosTop = (edgeTopLeft->vert->pos + edgeTopLeft->nextEdge->vert->pos + facePoint->pos)*0.25f;
					vec2 vecUVTop = (edgeTopLeft->vert->uv + edgeTopLeft->nextEdge->vert->uv + facePoint->uv)*0.25f;
					vec3 vecNormTop = (edgeTopLeft->vert->norm + edgeTopLeft->nextEdge->vert->norm + facePoint->norm)*0.25f;
					edgePointTop->pos = vecPosTop;
					edgePointTop->uv = vecUVTop;
					edgePointTop->norm = vecNormTop;
					
				}
				
				if(edgeRightTop->oppositeEdge->nextEdge->oppositeEdge->nextEdge->oppositeEdge == nullptr)
				{
					edgePointRight = edgeRightTop->oppositeEdge->nextEdge->oppositeEdge->nextEdge->vert;
					edgePointRight->pos = edgePointRight->pos+(facePoint->pos *0.25f);
					edgePointRight->uv = edgePointRight->uv+(facePoint->uv*0.25f);
					edgePointRight->norm = vec3::Normalize((edgePointRight->norm + (facePoint->norm *0.25f)));
					edgeRightBot->oppositeEdge = edgeRightTop->oppositeEdge;
					edgeRightTop->oppositeEdge->oppositeEdge = edgeRightBot;
					edgeRightTop->oppositeEdge = edgeRightBot->oppositeEdge->nextEdge->oppositeEdge->nextEdge;
					edgeRightTop->oppositeEdge->oppositeEdge = edgeRightTop;
				}
				else
				{
					edgePointRight = new HalfEdges::Vertex();
					vertices.Append(edgePointRight);
					edgePointRight->edge = edgeRightBot;
					vec3 vecPosRight = (edgeRightTop->vert->pos + edgeRightTop->nextEdge->vert->pos + facePoint->pos)*0.25f;
					vec2 vecUVRight = (edgeRightTop->vert->uv + edgeRightTop->nextEdge->vert->uv + facePoint->uv)*0.25f;
					vec3 vecNormRight = (edgeRightTop->vert->norm + edgeRightTop->nextEdge->vert->norm + facePoint->norm)*0.25f;
					edgePointRight->pos = vecPosRight;
					edgePointRight->uv = vecUVRight;
					edgePointRight->norm = vecNormRight;
				}
				
				if(edgeBotRight->oppositeEdge->nextEdge->oppositeEdge->nextEdge->oppositeEdge == nullptr)	
				{
					edgePointBot = edgeBotRight->oppositeEdge->nextEdge->oppositeEdge->nextEdge->vert;
					edgePointBot->pos = edgePointBot->pos+(facePoint->pos *0.25f);
					edgePointBot->uv = edgePointBot->uv+(facePoint->uv*0.25f);
					edgePointBot->norm = vec3::Normalize((edgePointBot->norm + (facePoint->norm *0.25f)));
					edgeBotLeft->oppositeEdge = edgeBotRight->oppositeEdge;
					edgeBotRight->oppositeEdge->oppositeEdge = edgeBotLeft;
					edgeBotRight->oppositeEdge = edgeBotLeft->oppositeEdge->nextEdge->oppositeEdge->nextEdge;
					edgeBotRight->oppositeEdge->oppositeEdge = edgeBotRight;
				}
				else
				{
					edgePointBot = new HalfEdges::Vertex();
					vertices.Append(edgePointBot);
					edgePointBot->edge = edgeBotLeft;
					vec3 vecPosBot = (edgeBotRight->vert->pos + edgeBotRight->nextEdge->vert->pos + facePoint->pos)*0.25f;
					vec2 vecUVBot = (edgeBotRight->vert->uv + edgeBotRight->nextEdge->vert->uv + facePoint->uv)*0.25f;
					vec3 vecNormBot = (edgeBotRight->vert->norm + edgeBotRight->nextEdge->vert->norm + facePoint->norm)*0.25f;
					edgePointBot->pos = vecPosBot;
					edgePointBot->uv = vecUVBot;
					edgePointBot->norm = vecNormBot;
				}
				
				if(edgeLeftBot->oppositeEdge->nextEdge->oppositeEdge->nextEdge->oppositeEdge == nullptr)
				{
					edgePointLeft = edgeLeftBot->oppositeEdge->nextEdge->oppositeEdge->nextEdge->vert;
					edgePointLeft->pos = edgePointLeft->pos+(facePoint->pos *0.25f);
					edgePointLeft->uv = edgePointLeft->uv+(facePoint->uv*0.25f);
					edgePointLeft->norm = vec3::Normalize((edgePointLeft->norm + (facePoint->norm *0.25f)));
					edgeLeftTop->oppositeEdge = edgeLeftBot->oppositeEdge;
					edgeLeftBot->oppositeEdge->oppositeEdge = edgeLeftTop;
					edgeLeftBot->oppositeEdge = edgeLeftTop->oppositeEdge->nextEdge->oppositeEdge->nextEdge;
					edgeLeftBot->oppositeEdge->oppositeEdge = edgeLeftBot;
					
				}
				else
				{
					edgePointLeft = new HalfEdges::Vertex();
					vertices.Append(edgePointLeft);
					edgePointLeft->edge = edgeLeftTop;
					vec3 vecPosLeft = (edgeLeftBot->vert->pos + edgeLeftBot->nextEdge->vert->pos + facePoint->pos)*0.25f;
					vec2 vecUVLeft = (edgeLeftBot->vert->uv + edgeLeftBot->nextEdge->vert->uv + facePoint->uv)*0.25f;
					vec3 vecNormLeft = (edgeLeftBot->vert->norm + edgeLeftBot->nextEdge->vert->norm + facePoint->norm)*0.25f;
					edgePointLeft->pos = vecPosLeft;
					edgePointLeft->uv = vecUVLeft;
					edgePointLeft->norm = vecNormLeft;
				}
				
				////Initialize edges for new faces////
				northWest->faceEdge = edgeTopLeft;
				northEast->faceEdge = edgeRightTop;
				southWest->faceEdge = edgeLeftBot;
				southEast->faceEdge = edgeBotRight;
				
				////NorthWest Face/////
				edgeTopLeft->face = northWest;
				edgeTopLeft->nextEdge = edgeTopMid;
				
				edgeTopMid->vert = edgePointTop;
				edgeTopMid->face = northWest;
				edgeTopMid->nextEdge = edgeMidLeft;
				edgeTopMid->oppositeEdge = edgeMidTop;
				
				edgeMidLeft->vert = facePoint;
				edgeMidLeft->face = northWest;
				edgeMidLeft->nextEdge = edgeLeftTop;
				edgeMidLeft->oppositeEdge = edgeLeftMid;
				
				edgeLeftTop->vert = edgePointLeft;
				edgeLeftTop->face = northWest;
				edgeLeftTop->nextEdge = edgeTopLeft;
				//////////////////////
				
				////NorthEast Face////
				edgeTopRight->vert = edgePointTop;
				edgeTopRight->face = northEast;
				edgeTopRight->nextEdge = edgeRightTop;
				
				edgeRightTop->face = northEast;
				edgeRightTop->nextEdge = edgeRightMid;
				
				edgeRightMid->vert = edgePointRight;
				edgeRightMid->face = northEast;
				edgeRightMid->nextEdge = edgeMidTop;
				edgeRightMid->oppositeEdge = edgeMidRight;
				
				edgeMidTop->vert = facePoint;
				edgeMidTop->face = northEast;
				edgeMidTop->nextEdge = edgeTopRight;
				edgeMidTop->oppositeEdge = edgeTopMid;
				///////////////////////
				
				////SouthEast Face////
				edgeMidRight->vert = facePoint;
				edgeMidRight->face = southEast;
				edgeMidRight->nextEdge = edgeRightBot;
				edgeMidRight->oppositeEdge = edgeRightMid;
				
				edgeRightBot->vert = edgePointRight;
				edgeRightBot->face = southEast;
				edgeRightBot->nextEdge = edgeBotRight;
				
				edgeBotRight->face = southEast;
				edgeBotRight->nextEdge = edgeBotMid;
				
				edgeBotMid->vert = edgePointBot;
				edgeBotMid->face = southEast;
				edgeBotMid->nextEdge = edgeMidRight;
				edgeBotMid->oppositeEdge = edgeMidBot;
				////////////////////////
				
				////southWest Face////
				edgeLeftMid->vert = edgePointLeft;
				edgeLeftMid->face = southWest;
				edgeLeftMid->nextEdge = edgeMidBot;
				edgeLeftMid->oppositeEdge = edgeMidLeft;
				
				edgeMidBot->vert = facePoint;
				edgeMidBot->face = southWest;
				edgeMidBot->nextEdge = edgeBotLeft;
				edgeMidBot->oppositeEdge = edgeBotMid;
				
				edgeBotLeft->vert = edgePointBot;
				edgeBotLeft->face = southWest;
				edgeBotLeft->nextEdge = edgeLeftBot;

				edgeLeftBot->face = southWest;
				edgeLeftBot->nextEdge = edgeLeftMid;
				/////////////////////////

			}
			float valence = 0.0f;
			for(int i = 0; i < original_pos.size(); ++i)
			{
				
				vec3 avg_pos;
				vec3 avgPoint_pos;
				
				if(i < 8)
				{
					valence = 3.0f;
					HalfEdges::HalfEdge* vertEdge = vertices[i]->edge;
					HalfEdges::Vertex* startVert = vertices[i];
					HalfEdges::Vertex* firstVert = vertEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* secondVert = vertEdge->nextEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* thirdVert = vertEdge->nextEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					
					avg_pos = (firstVert->pos + secondVert->pos + thirdVert->pos)/valence;
					
					HalfEdges::Vertex* firstPoint = vertEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* secondPoint = vertEdge->oppositeEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* thirdPoint = vertEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->nextEdge->vert;
					
					avgPoint_pos = (((firstPoint->pos + startVert->pos)*0.5f) + ((secondPoint->pos + startVert->pos)*0.5f) + ((thirdPoint->pos+ startVert->pos)*0.5f))/valence;
				}
				else
				{
					valence = 4.0f;
					HalfEdges::HalfEdge* vertEdge = vertices[i]->edge;
					HalfEdges::Vertex* startVert = vertices[i];
					HalfEdges::Vertex* firstVert = vertEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* secondVert = vertEdge->nextEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* thirdVert = vertEdge->nextEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* fourthVert = vertEdge->oppositeEdge->nextEdge->nextEdge->nextEdge->vert;
					
					avg_pos = (firstVert->pos + secondVert->pos + thirdVert->pos + fourthVert->pos)/valence;
					
					HalfEdges::Vertex* firstPoint = vertEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* secondPoint = vertEdge->oppositeEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* thirdPoint = vertEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->nextEdge->vert;
					HalfEdges::Vertex* fourthPoint = vertEdge->oppositeEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->oppositeEdge->nextEdge->nextEdge->vert;
					
					avgPoint_pos = (((firstPoint->pos + startVert->pos)*0.5f) + ((secondPoint->pos + startVert->pos)*0.5f) + ((thirdPoint->pos+ startVert->pos)*0.5f) + ((fourthPoint->pos+ startVert->pos)*0.5f))/valence;
					
				}
				
				float m1 = (valence - 3.0f) / valence;
				float m2 = 1.0f / valence;
				float m3 = 2.0f / valence;
				
				vertices[i]->pos = (original_pos[i] * m1) + (avg_pos * m2)+ (avgPoint_pos * m3);
			}
			
			float timeStamp = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - before).count() / 1000.0f);
			printf("Time for iteration %i: %f \n", iterationCount, timeStamp);
		}
		float stop = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0f);
		printf("Total time for %i iterations: %f \n", iterations, stop);
	}



}
