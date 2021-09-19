##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=nogui
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
Date                   :=19/09/2021
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
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall -std=c++14  $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=./build-$(ConfigurationName)//src_nogui.cpp$(ObjectSuffix) ./build-$(ConfigurationName)//src_expression.cpp$(ObjectSuffix) 



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
./build-$(ConfigurationName)//src_nogui.cpp$(ObjectSuffix): src/nogui.cpp ./build-$(ConfigurationName)//src_nogui.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "H:/Documents/c++/calculator/src/nogui.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_nogui.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//src_nogui.cpp$(DependSuffix): src/nogui.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//src_nogui.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//src_nogui.cpp$(DependSuffix) -MM src/nogui.cpp

./build-$(ConfigurationName)//src_nogui.cpp$(PreprocessSuffix): src/nogui.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//src_nogui.cpp$(PreprocessSuffix) src/nogui.cpp

./build-$(ConfigurationName)//src_expression.cpp$(ObjectSuffix): src/expression.cpp ./build-$(ConfigurationName)//src_expression.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "H:/Documents/c++/calculator/src/expression.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_expression.cpp$(ObjectSuffix) $(IncludePath)
./build-$(ConfigurationName)//src_expression.cpp$(DependSuffix): src/expression.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT./build-$(ConfigurationName)//src_expression.cpp$(ObjectSuffix) -MF./build-$(ConfigurationName)//src_expression.cpp$(DependSuffix) -MM src/expression.cpp

./build-$(ConfigurationName)//src_expression.cpp$(PreprocessSuffix): src/expression.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ./build-$(ConfigurationName)//src_expression.cpp$(PreprocessSuffix) src/expression.cpp


-include ./build-$(ConfigurationName)///*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


