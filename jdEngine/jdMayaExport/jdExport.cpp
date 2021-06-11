#include <jdSaveData.h>
#include <jdVector4.h>
#include <string.h> 
#include <sys/types.h>
#include <maya/MStatus.h>
#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSet.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshEdge.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MObjectArray.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MDistance.h>
#include <maya/MIntArray.h>
#include <maya/MIOStream.h>
#include <maya/MMatrix.h>
#include <fstream>
#include <vector>
#include <jdMesh.h>

static MSpace::Space g_space = MSpace::Space::kObject;

/**
	* @brief Mesh only for maya export
	*/
struct Mesh {
		std::vector<jdEngineSDK::DefaultVertexData> vertexInfo;
		std::vector<jdEngineSDK::uint32> indexInfo;
		std::string name;
};

/**
	* @brief Model only for maya export
	*/
struct Model {
		std::vector<Mesh> meshes;
};

/**
	* @brief class for maya export
	*/
class JDTranslator : public MPxFileTranslator
{
public:

		/**
			* @brief default constructor based in MPxFileTraslator
			*/
		JDTranslator() : MPxFileTranslator() {};

		/**
			* @brief default desconstructor
			*/
		~JDTranslator() {};

		/**
			* @brief file export operations
			* @note we overload this method to perform any, file export operations needed
			*/
		MStatus
		writer(const MFileObject& file,
						   const MString& optionsString,
						   FileAccessMode mode);

		/**
			* @brief To check if can export
			* @return returns true if this class can export files
			*/
		bool
		haveWriteMethod() const {
				return true;
		}

		/**
			* @brief To check if can import
			* @return returns true if this class can import files
			*/
		bool
		haveReadMethod() const {
				return false;
		};

		/**
			* @brief To get the default extention
			* @return the default extension of the file supported by this FileTranslator.
			*/
		MString defaultExtension() const {

				return "jdm";
		}

		/**
			* @brief Used by Maya to create a new instance of our custom file translator
			*/
		static void* creator() {

				return new JDTranslator;
		}

		// some option flags set by the mel script
		bool m_bOption1;
		bool m_bOption2;
};

const char* const g_OptionScript = "jdExportOptions";
// the default option string
const char* const g_DefaultExportOptions = { "WorldSpace = 1" };


class ComandWorldSpace : public MPxCommand {
 public:
		ComandWorldSpace() {};

		virtual MStatus	doIt(const MArgList&) {
				g_space = MSpace::Space::kWorld;
				//std::cout.rdbuf(std::cerr.rdbuf("Change to WORLD SPACE"));
				std::cout << "Change to WORLD SPACE" << std::endl;
				return MS::kSuccess;
		};
		
		static void* creator() {
				return new ComandWorldSpace;
		};
};

class ComandModelSpace : public MPxCommand {
 public:
		ComandModelSpace() {};

		virtual MStatus	doIt(const MArgList&) {
				g_space = MSpace::Space::kObject;
				std::cout << "Change to OBJECT SPACE" << std::endl;
				return MS::kSuccess;
		};

		static void* creator() {
				return new ComandModelSpace;
		};
};

MStatus initializePlugin(MObject obj)
{
		MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");
		MStatus stat = plugin.registerFileTranslator("JDexport", "none",
				                                           JDTranslator::creator,
				                                           (char*)g_OptionScript,
				                                           (char*)g_DefaultExportOptions);

		if (!stat)	{
				stat.perror("registerFileTranslator");
				return stat;
		}
		stat = plugin.registerCommand("CmdMSpace",					
	                               ComandModelSpace::creator );	    
		if (!stat) {
				stat.perror("registerCommandModelSpace");
				return stat;
		}
		stat = plugin.registerCommand("CmdWSpace",					
	                               ComandWorldSpace::creator );	    
		if (!stat) {
				stat.perror("registerCommandModelSpace");
				return stat;
		}
		return stat;
		// Register the translator with the system
		//return plugin.registerFileTranslator("JDexport", "none",
		//		                                   JDTranslator::creator,
		//		                                   (char*)g_OptionScript,
		//		                                   (char*)g_DefaultExportOptions);
}

MStatus uninitializePlugin(MObject obj) {
		MFnPlugin plugin(obj);
		MStatus stat = plugin.deregisterFileTranslator("JDexport");
		if (!stat) {
				stat.perror("deregisterFileTranslator");
				return stat;
		}
		stat = plugin.deregisterCommand("CmdMSpace");
		if (!stat) {
				stat.perror("deregisterCommandModelSpace");
				return stat;
		}
		stat = plugin.deregisterCommand("CmdWSpace");
		if (!stat) {
				stat.perror("deregisterCommandModelSpace");
				return stat;
		}
		return stat;
		//return plugin.deregisterFileTranslator("JDexport");
}

MStatus
JDTranslator::writer(const MFileObject& file, const MString& optionsString, FileAccessMode mode) {

		MStatus stat;
		Model* tempModel = new	Model;
		MItDag dagIter(MItDag::kBreadthFirst, MFn::kInvalid, &stat);

		for (; !dagIter.isDone(); dagIter.next())
		{
				MDagPath dagPath;
				stat = dagIter.getPath(dagPath);

				if (stat)
				{
						MFnDagNode dagNode(dagPath, &stat);
						//Check if is a mesh
						if (dagNode.isIntermediateObject()) continue;
						if (!dagPath.hasFn(MFn::kMesh)) continue;
						if (dagPath.hasFn(MFn::kTransform)) continue;

						// get the mesh
						MFnMesh fnMesh(dagPath);
						//Create a mesh to save data
						Mesh newMesh;
						newMesh.name = fnMesh.name().asChar();

						MSpace::Space space = g_space;
						//Get vertex info
						MPointArray vertexList;
						fnMesh.getPoints(vertexList, space);


						// Get UVSets for this mesh
						MStringArray  UVSets;
						stat = fnMesh.getUVSetNames(UVSets);

						//Get polygons of mesh
						MObject comp;
						MItMeshPolygon piter(dagPath, comp);

						// now iterate over all the polygons in the mesh
						for (; !piter.isDone(); piter.next())	{
								//Get UV coordinate of polygon
								MFloatArray poliUArray, poliVArray;
								piter.getUVs(poliUArray, poliVArray, &UVSets[0]);

								//get index's vertex of current polygon
								MIntArray iA;
								piter.getVertices(iA);
								jdEngineSDK::uint32 indexLength = iA.length();

								for (size_t i = 0; i < indexLength; i++)	{
										jdEngineSDK::DefaultVertexData newVertex;
										//get position data
										MPoint Point = vertexList[iA[i]];
										//get normal data
										MVector N;
										fnMesh.getFaceVertexNormal(piter.index(), iA[i], N, space);
										//get tangent data
										MVector T;
										fnMesh.getFaceVertexTangent(piter.index(), iA[i], T, space);
										//Set all
										newVertex.Pos[0] = Point.x;
										newVertex.Pos[1] = Point.y;
										newVertex.Pos[2] = -Point.z;
										newVertex.Pos[3] = Point.w;
										newVertex.Norm[0] = N.x;
										newVertex.Norm[1] = N.y;
										newVertex.Norm[2] = -N.z;
										newVertex.Norm[3] = 0;
										newVertex.Tang[0] = T.x;
										newVertex.Tang[1] = T.y;
										newVertex.Tang[2] = -T.z;
										newVertex.Tang[3] = 0;
										newVertex.TexCoord.x = poliUArray[i];
										newVertex.TexCoord.y = 1 - poliVArray[i];
										newMesh.vertexInfo.push_back(newVertex);
								}
								//Create index for a quad or tri
								if (4 == indexLength)	{
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 4);
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 2);
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 3);

										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 4);
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 1);
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 2);
								}
								else if (3 == indexLength)	{
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 3);
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 1);
										newMesh.indexInfo.push_back(newMesh.vertexInfo.size() - 2);
								}
								
						}
						//Push the mesh
						tempModel->meshes.push_back(newMesh);
				}
		}
		//open file
		std::ofstream projectFile(file.expandedFullName().asChar(), std::ios::binary);
		if (projectFile)	{
				//Save all data
				jdEngineSDK::MODEL_INFO modelinfo;
				jdEngineSDK::uint32 numMeshs = tempModel->meshes.size();
				modelinfo.numMeshes = numMeshs;
				modelinfo.numAnimations = 0;
				modelinfo.animationNodes = false;

				projectFile.write((char*)&modelinfo, sizeof(jdEngineSDK::MODEL_INFO));

				for (jdEngineSDK::uint32 m = 0; m < numMeshs; ++m) {
						auto cMesh = tempModel->meshes[m];
						jdEngineSDK::MESH_INFO meshInfo;
						meshInfo.nameSize = cMesh.name.size();

						meshInfo.numVertex = cMesh.vertexInfo.size();
						meshInfo.numIndex = cMesh.indexInfo.size();
						meshInfo.numBonesTransform = 0;
						meshInfo.numBones = 0;
						meshInfo.num_bonesMap = 0;
						meshInfo.numBonesInfo = 0;
						meshInfo.albedoID = 0;
						meshInfo.normalID = 0;
						meshInfo.specID = 0;
						meshInfo.roughnessID = 0;

						projectFile.write((char*)&meshInfo, sizeof(jdEngineSDK::MESH_INFO));
						projectFile.write((char*)cMesh.name.data(), sizeof(char) * meshInfo.nameSize);

						projectFile.write((char*)cMesh.vertexInfo.data(),
								sizeof(jdEngineSDK::DefaultVertexData) * meshInfo.numVertex);
						//write index
						projectFile.write((char*)cMesh.indexInfo.data(), sizeof(jdEngineSDK::uint32) * meshInfo.numIndex);
				}
				projectFile.close();
		}
		delete tempModel;
		return stat;
}
