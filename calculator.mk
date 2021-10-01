##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=calculator
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=H:/Documents/c++/calculator
ProjectPath            :=H:/Documents/c++/calculator
IntermediateDirectory  :=./build-$(ConfigurationName)/
OutDir                 :=./build-$(ConfigurationName)/
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=19maho
Date                   :=01/10/2021
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=.\build-$(ConfigurationName)\bin\$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  res\resource.res
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)wxbase31u $(LibrarySwitch)wxbase31u_net $(LibrarySwitch)wxbase31u_xml $(LibrarySwitch)wxexpat $(LibrarySwitch)wxjpeg $(LibrarySwitch)wxmsw31u_adv $(LibrarySwitch)wxmsw31u_aui $(LibrarySwitch)wxmsw31u_core $(LibrarySwitch)wxmsw31u_gl $(LibrarySwitch)wxmsw31u_html $(LibrarySwitch)wxmsw31u_media $(LibrarySwitch)wxmsw31u_propgrid $(LibrarySwitch)wxmsw31u_ribbon $(LibrarySwitch)wxmsw31u_richtext $(LibrarySwitch)wxmsw31u_stc $(LibrarySwitch)wxmsw31u_webview $(LibrarySwitch)wxmsw31u_xrc $(LibrarySwitch)wxpng $(LibrarySwitch)wxregexu $(LibrarySwitch)wxscintilla $(LibrarySwitch)wxtiff $(LibrarySwitch)wxzlib 
ArLibs                 :=  "libwxbase31u.a" "libwxbase31u_net.a" "libwxbase31u_xml.a" "libwxexpat.a" "libwxjpeg.a" "libwxmsw31u_adv.a" "libwxmsw31u_aui.a" "libwxmsw31u_core.a" "libwxmsw31u_gl.a" "libwxmsw31u_html.a" "libwxmsw31u_media.a" "libwxmsw31u_propgrid.a" "libwxmsw31u_ribbon.a" "libwxmsw31u_richtext.a" "libwxmsw31u_stc.a" "libwxmsw31u_webview.a" "libwxmsw31u_xrc.a" "libwxpng.a" "libwxregexu.a" "libwxscintilla.a" "libwxtiff.a" "libwxzlib.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=./build-$(ConfigurationName)//src_expression.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//src_vector.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//src_main.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//src_graphcanvas.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ./build-$(ConfigurationName)//.d $(Objects) 
	@if not exist ".\build-$(ConfigurationName)\" mkdir ".\build-$(ConfigurationName)\"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist ".\build-$(ConfigurationName)\" mkdir ".\build-$(ConfigurationName)\"
	@if not exist "".\build-$(ConfigurationName)\bin"" mkdir "".\build-$(ConfigurationName)\bin""

./build-$(ConfigurationName)//.d:
	@if not exist ".\build-$(ConfigurationName)\" mkdir ".\build-$(ConfigurationName)\"

PreBuild:


##
## Objects
##
./build-$(ConfigurationName)//src_expression.cpp$(ObjectSuffix): src/expression.cpp ./build-$(ConfigurationName)//src_expression.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "H:/Documents/c++/calculator/src/expression.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_expression.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//src_expression.cpp$(DependSuffix): src/expression.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//src_expression.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//src_expression.cpp$(DependSuffix) -MM src/expression.cpp

./build-$(ConfigurationName)//src_expression.cpp$(PreprocessSuffix): src/expression.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//src_expression.cpp$(PreprocessSuffix) src/expression.cpp

./build-$(ConfigurationName)//src_vector.cpp$(ObjectSuffix): src/vector.cpp ./build-$(ConfigurationName)//src_vector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "H:/Documents/c++/calculator/src/vector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vector.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//src_vector.cpp$(DependSuffix): src/vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//src_vector.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//src_vector.cpp$(DependSuffix) -MM src/vector.cpp

./build-$(ConfigurationName)//src_vector.cpp$(PreprocessSuffix): src/vector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//src_vector.cpp$(PreprocessSuffix) src/vector.cpp

./build-$(ConfigurationName)//src_main.cpp$(ObjectSuffix): src/main.cpp ./build-$(ConfigurationName)//src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "H:/Documents/c++/calculator/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//src_main.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//src_main.cpp$(DependSuffix) -MM src/main.cpp

./build-$(ConfigurationName)//src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//src_main.cpp$(PreprocessSuffix) src/main.cpp

./build-$(ConfigurationName)//src_graphcanvas.cpp$(ObjectSuffix): src/graphcanvas.cpp ./build-$(ConfigurationName)//src_graphcanvas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "H:/Documents/c++/calculator/src/graphcanvas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_graphcanvas.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//src_graphcanvas.cpp$(DependSuffix): src/graphcanvas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//src_graphcanvas.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//src_graphcanvas.cpp$(DependSuffix) -MM src/graphcanvas.cpp

./build-$(ConfigurationName)//src_graphcanvas.cpp$(PreprocessSuffix): src/graphcanvas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//src_graphcanvas.cpp$(PreprocessSuffix) src/graphcanvas.cpp


-include ./build-$(ConfigurationName)///*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


