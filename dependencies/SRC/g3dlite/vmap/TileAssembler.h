/***
 * Demonstrike Core
 */

#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>

namespace VMAP
{
	/**
	This Class is used to convert raw vector data into balanced BSP-Trees.
	To start the conversion call convertWorld().
	*/
	//===============================================

	class ModelPosition
	{
		private:
			G3D::Matrix3 iRotation;
		public:
			G3D::Vector3 iPos;
			G3D::Vector3 iDir;
			float iScale;
			void init()
			{
				iRotation = G3D::Matrix3::fromEulerAnglesZYX(G3D::pi()*iDir.y/180.f, G3D::pi()*iDir.x/180.f, G3D::pi()*iDir.z/180.f);
			}
			G3D::Vector3 transform(const G3D::Vector3& pIn) const;
			void moveToBasePos(const G3D::Vector3& pBasePos) { iPos -= pBasePos; }
	};

	typedef std::map<G3D::g3d_uint32, ModelSpawn> UniqueEntryMap;
	typedef std::multimap<G3D::g3d_uint32, G3D::g3d_uint32> TileMap;

	struct MapSpawns
	{
		UniqueEntryMap UniqueEntries;
		TileMap TileEntries;
	};

    typedef std::map<G3D::g3d_uint32, MapSpawns*> MapData;
	//===============================================

	class TileAssembler
	{
		private:
			std::string iDestDir;
			std::string iSrcDir;
			bool (*iFilterMethod)(char *pName);
			G3D::Table<std::string, unsigned int > iUniqueNameIds;
			unsigned int iCurrentUniqueNameId;
			MapData mapData;

		public:
			TileAssembler(const std::string& pSrcDirName, const std::string& pDestDirName);
			virtual ~TileAssembler();

			bool convertWorld2();
			bool readMapSpawns();
			bool calculateTransformedBound(ModelSpawn &spawn);

			bool convertRawFile(const std::string& pModelFilename);
			void setModelNameFilterMethod(bool (*pFilterMethod)(char *pName)) { iFilterMethod = pFilterMethod; }
			std::string getDirEntryNameFromModName(unsigned int pMapId, const std::string& pModPosName);
			unsigned int getUniqueNameId(const std::string pName);
	};

}															// VMAP
