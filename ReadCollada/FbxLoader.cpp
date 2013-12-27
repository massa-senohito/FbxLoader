#include "FbxLoader.h"
#include <functional>
#include <list>
#include <algorithm>
#include <memory>
#include <iostream>
#include <locale>
using std::unique_ptr;
void PrintTrans(_Transform& trans){
    //typedef std::num_get<char> numg;
    std::cout <<  trans.Pos.X << std::endl;
    std::cout <<  trans.Pos.Y << std::endl;
}
FbxLoader::FbxLoader(void)
{
/* FbxManager *lSdkManager = FbxManager::Create();
*
* // create an IOSettings object
* FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
*
* // set some IOSettings options 
* ios->SetBoolProp(IMP_FBX_MATERIAL, true);
* ios->SetBoolProp(IMP_FBX_TEXTURE,  true);
*
* // create an empty scene
* FbxScene* lScene = FbxScene::Create(lSdkManager,"");
*
* // Create an importer.
* FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
*
* // Initialize the importer by providing a filename and the IOSettings to use
* lImporter->Initialize("C:\\myfile.fbx", -1, ios);
*
* // Import the scene.
* lImporter->Import(lScene); 
*
* // Destroy the importer.
* lImporter->Destroy(); 
*/
    int major, miner,  rev;
    manager=FbxManager::Create();
    FbxManager::GetFileFormatVersion(major,miner,rev);

    ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);

    scene=FbxScene::Create(manager,"myscene");
    // Create an importer using the SDK manager.
    importer = FbxImporter::Create(manager,"");

    // Use the first argument as the filename for the importer.

}

unique_ptr<char> getUTF8AbsFilePath( const char* filePath
                         )                     
 //                       , char outAbsFilePath[ 512 ] ) 
{
   char fullPath[ 512 ];
   unique_ptr<char>
   //char *
     outAbsFilePath=unique_ptr<char>( );
   size_t size=sizeof(fullPath);
   char *out=outAbsFilePath.get();
   _fullpath( fullPath, filePath, 512 );
   FbxAnsiToUTF8( fullPath, out ,&size);
   outAbsFilePath=out;
   return outAbsFilePath;
}
//
void FbxLoader::Load(std::string file,LoadFun f)
{
    //std::string filename="C:\\Users\\massa_senohito\\Documents\\Visual\ Studio\ 11\\Projects\\ReadCollada\\ReadCollada";
    //filename    += "cube.fbx";
    char filename[512];
    auto p=getUTF8AbsFilePath("cube.fbx");
    //daeやobjなどにも対応している、詳しくはMyOwnReaderWriterを参照のこと
    if(!importer->Initialize(p.get(), -1, ios)) { 
        auto code=importer->GetStatus().GetCode()==FbxStatus::eInvalidFile;
        printf("Call to FbxImporter::Initialize() failed.\n"); 
        printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString()); 
        char num;
        gets_s(&num,sizeof(char)) ;
    }
    int maj,miner,rev;
//todo use load macro or swig to debug
    importer->GetFileVersion(maj,miner,rev);
    importer->Import(scene);
    std::cout<<maj << "." << miner << "." << rev <<std::endl;
    //オブジェクトの数だけスレッドたてちゃうか
    int count=scene->GetSrcObjectCount();
    for(int i=0;i<count;i++){
        const auto obj=scene->GetSrcObject(i);
        f(obj);
    }
}

//アニメーションはシーンに0個以上存在する
void Animation(FbxAnimStack& stack){
    int layers=stack.GetMemberCount<FbxAnimLayer>();
    for (int i = 0; i < layers; i++)
    {
        auto l=stack.GetMember<FbxAnimLayer>();
	//ノードのローカルトランスフォームにレイヤーのカーブがあるので、みたいレイヤーを渡せばGetCurveできる
    }
}

//モデルデータ内の頂点に剛体情報などを仕込む
//最下層の表現では頂点のデータをリストに放り込む
//その上の層で頂点、剛体を扱うレコードにマップ

//オブジェクトが隠されているか調べます
bool ObjectHidden(FbxNode& node){
    //選択されているかも調べられる
    return node.GetObjectFlags(FbxObject::eHidden);
}
//ノードの親オブジェクトプロパティを読み込んで、頂点にその情報
void Property(const FbxObject* obj){
    //オブジェクトに設定されたプロパティを読み取り、
    auto prop=obj->GetFirstProperty();
    //アニメーションするか、ロックされているかも取れる
    if(prop.GetFlag(FbxPropertyAttr::eUserDefined)){
	//ラベルが空でも何も出力しない(internal nameが普通出力される)
        auto lab=prop.GetLabel(false);
        auto internalname=prop.GetName();
	//時間やdouble型color等が設定されている
        auto type=prop.GetPropertyDataType().GetType();
        switch (type)
        {
        case eFbxString://それが文字なら
            {
                auto s=prop.Get<FbxString>();
                std::cout<< s<< std::endl;
            }        
            break;
        default:
            std::cout<< type<< std::endl;

            break;
        }
    }
}

std::list<_Vector3*>* MeshTriangle(const FbxMesh& mesh,float size){
    auto point=new _Vector3();
    
    std::list<_Vector3*>* plist=new std::list<_Vector3*>();
    auto pCoord=mesh.GetControlPoints();
    for(int i=0;i<mesh.GetControlPointsCount();i++)
    {
        point->X=(float)pCoord[i][0]*size;
        point->Y=(float)pCoord[i][1]*size;
        point->Z=(float)pCoord[i][2]*size;
        plist->push_back(point);
    }
    //こいつへのポインタ返してるのでdeleteできない delete point;
    return plist;
}

/*
function walkTree(node) {
   if (node == null) // 
      return;
   // ノードに対して何かする
   for (var i = 0; i < node.childNodes.length; i++) {
      walkTree(node.childNodes[i]);
   }
}
*/



void walkNode(const FbxNode& node,std::function<void(const FbxNode&)> nodeAction){
    //return
    int nodes=node.GetChildCount(false);
    for(int i=0;i<nodes;i++){
        nodeAction(node);
	if(nodes==0) return;
    walkNode(*node.GetChild(i),nodeAction);
    }
}
    //http://stackoverflow.com/questions/17294629/merging-sub-vectors-int-a-single-vector-c

void PrintNode(FbxNode* node){
        //node->PostRotation.
  auto type=node->GetNodeAttribute()->GetAttributeType();
  decltype(node->GetMesh()) mesh;
  if(type==FbxNodeAttribute::eMesh)
    mesh=node->GetMesh();
}
_Vector3 vertexLoad(FbxNode& node){
    //オブジェクトのさらにローカル座標
    auto tr=node.LclTranslation.Get();
    _Vector3 v={tr[0],tr[1],tr[2]};
    return v;
}
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) { 
    switch(type) { 
        case FbxNodeAttribute::eUnknown: return "unidentified"; 
        case FbxNodeAttribute::eNull: return "null"; 
        case FbxNodeAttribute::eMarker: return "marker"; 
        case FbxNodeAttribute::eSkeleton: return "skeleton"; 
        case FbxNodeAttribute::eMesh: return "mesh"; 
        case FbxNodeAttribute::eNurbs: return "nurbs"; 
        case FbxNodeAttribute::ePatch: return "patch"; 
        case FbxNodeAttribute::eCamera: return "camera"; 
        case FbxNodeAttribute::eCameraStereo: return "stereo"; 
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
        case FbxNodeAttribute::eLight: return "light"; 
        case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
        case FbxNodeAttribute::eOpticalMarker: return "marker"; 
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
        case FbxNodeAttribute::eBoundary: return "boundary"; 
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
        case FbxNodeAttribute::eShape: return "shape"; 
        case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
        case FbxNodeAttribute::eSubDiv: return "subdiv"; 
        default: return "unknown"; 
    } 
}

//void
//  FbxLoader::EachNode(std::function<void(FbxNode*)> nodeAction){
//  auto root=scene->GetRootNode();
//  auto l=MakeNodeList(root);
//  std::for_each(l->begin(),l->end(),nodeAction);
//  delete l;
//}
FbxLoader::~FbxLoader(void)
{
    scene->Destroy();
    importer->Destroy();
    manager->Destroy();
}
// http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html
//たぶん3Dシーンをこれだけで構築できる
// vim:ts=2:expandtab