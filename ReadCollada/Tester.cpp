#include "Tester.h"
#include "FbxLoader.h"
#include <iostream>
//#include <filesystem>
#include <list>
#include <algorithm>
#include <string>
int main(int argc , char** argv){
    //filelist(argv);
    auto t=Tester();
  //delete t;
}

//std::list<const std::string*>* 
void filelist(char** arg){
  //using namespace std::tr2;
  //using namespace std;
  ////auto files=(new std::list<const std::string*>());
  //sys::path cur(arg[0]);
  //sys::directory_iterator it(cur.parent_path());
  //for(sys::directory_iterator i=it;i!=NULL;++i)
  //{
  //    sys::path pat(*i);
  //    cout<< pat.filename() <<endl;
  //}
  //return files;
}

Tester::Tester(void)
{
    using namespace std;
    //filelist();
    /*std::for_each(files.begin(),files.end(),[](const std::string* n){
        cout << n <<endl;
    });
    std::for_each(files.begin(),files.end(),[](const std::string* i){
	delete i;
    });
    files.clear();*/
    FbxLoader loder= FbxLoader();
    //‚Þ‚µ‚ëcollada‚Æ‚©‚ðƒeƒXƒg‚µ‚½•û‚ª‚¢‚¢‚©‚à
    loder.Load("cube.fbx",Property);
        char num;
        gets_s(&num,sizeof(char)) ;
    //{
    //static int c=0;
    //    std::cout<< c++<<std::endl;
    //});
}


Tester::~Tester(void)
{
    
}
