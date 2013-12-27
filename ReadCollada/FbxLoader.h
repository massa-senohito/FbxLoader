#pragma once

#include <string>
#include "fbxsdk.h"
typedef void(*LoadFun)(const FbxObject*); 

using namespace fbxsdk_2014_1;
void DrawMesh(FbxMesh*,FbxAMatrix*,FbxPose*);
//template<typename T>
//void deleteListAndElem(std::list<T*>* list){
//  //sefedelete
//}
struct _Vector3{
  float X;float Y;float Z;

};
struct _Transform{
    _Vector3 Pos;
    _Vector3 Scale;
    _Vector3 Rot;
};
void PrintTrans(_Transform& trans);
void Property(const FbxObject* node);
class FbxLoader
{
	FbxScene* scene;
	FbxManager* manager;
	FbxImporter* importer;
    FbxIOSettings* ios;
public:
	FbxLoader(void);
	~FbxLoader(void);
	void Load(std::string,LoadFun);
	//void EachNode(std::function<void(FbxNode*)>);
};

