##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=CCP
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/michi/OneDrive/RNgit/RNp2
ProjectPath            :=/home/michi/OneDrive/RNgit/RNp2/CCP
IntermediateDirectory  :=../build-$(ConfigurationName)/CCP
OutDir                 :=../build-$(ConfigurationName)/CCP
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=michi
Date                   :=03/12/19
CodeLitePath           :=/home/michi/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
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
OutputFile             :=../build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  -pthread
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
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=../build-$(ConfigurationName)/CCP/main.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/server.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/client.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/CCP/.d $(Objects) 
	@mkdir -p "../build-$(ConfigurationName)/CCP"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "../build-$(ConfigurationName)/CCP"
	@mkdir -p ""../build-$(ConfigurationName)/bin""

../build-$(ConfigurationName)/CCP/.d:
	@mkdir -p "../build-$(ConfigurationName)/CCP"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/CCP/main.c$(ObjectSuffix): main.c ../build-$(ConfigurationName)/CCP/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/main.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/main.c$(DependSuffix) -MM main.c

../build-$(ConfigurationName)/CCP/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/main.c$(PreprocessSuffix) main.c

../build-$(ConfigurationName)/CCP/ccp.c$(ObjectSuffix): ccp.c ../build-$(ConfigurationName)/CCP/ccp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp.c$(DependSuffix): ccp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp.c$(DependSuffix) -MM ccp.c

../build-$(ConfigurationName)/CCP/ccp.c$(PreprocessSuffix): ccp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp.c$(PreprocessSuffix) ccp.c

../build-$(ConfigurationName)/CCP/server.c$(ObjectSuffix): server.c ../build-$(ConfigurationName)/CCP/server.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/server.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/server.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/server.c$(DependSuffix): server.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/server.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/server.c$(DependSuffix) -MM server.c

../build-$(ConfigurationName)/CCP/server.c$(PreprocessSuffix): server.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/server.c$(PreprocessSuffix) server.c

../build-$(ConfigurationName)/CCP/client.c$(ObjectSuffix): client.c ../build-$(ConfigurationName)/CCP/client.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/client.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/client.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/client.c$(DependSuffix): client.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/client.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/client.c$(DependSuffix) -MM client.c

../build-$(ConfigurationName)/CCP/client.c$(PreprocessSuffix): client.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/client.c$(PreprocessSuffix) client.c


-include ../build-$(ConfigurationName)/CCP//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


