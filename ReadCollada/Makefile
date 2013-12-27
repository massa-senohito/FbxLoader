COM=clang++
FbxLoader.obj:
	$(COM) -w -I"D:\Program Files\Autodesk\FBX\FBX SDK\2014.1\include" -cxx-isystem"D:\Program Files\Autodesk\FBX\FBX SDK\2014.1\lib\vs2012\x86\debug" -v -MTlibfbxsdk.lib FbxLoader.cpp 
Tester.obj:
	$(COM) Tester.cpp -o Tester.obj
All:
	$(COM) -o FbxTest.exe Tester.obj FbxLoader.obj

