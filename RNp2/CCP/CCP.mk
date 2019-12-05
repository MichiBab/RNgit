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
Date                   :=05/12/19
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
Objects0=../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/server.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/main.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/msgflags.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(ObjectSuffix) ../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(ObjectSuffix) \
	../build-$(ConfigurationName)/CCP/client.c$(ObjectSuffix) 



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
../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(ObjectSuffix): ccp_create_packs.c ../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_create_packs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_create_packs.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(DependSuffix): ccp_create_packs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(DependSuffix) -MM ccp_create_packs.c

../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(PreprocessSuffix): ccp_create_packs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_create_packs.c$(PreprocessSuffix) ccp_create_packs.c

../build-$(ConfigurationName)/CCP/server.c$(ObjectSuffix): server.c ../build-$(ConfigurationName)/CCP/server.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/server.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/server.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/server.c$(DependSuffix): server.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/server.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/server.c$(DependSuffix) -MM server.c

../build-$(ConfigurationName)/CCP/server.c$(PreprocessSuffix): server.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/server.c$(PreprocessSuffix) server.c

../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(ObjectSuffix): ccp_react_routine.c ../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_react_routine.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_react_routine.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(DependSuffix): ccp_react_routine.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(DependSuffix) -MM ccp_react_routine.c

../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(PreprocessSuffix): ccp_react_routine.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_react_routine.c$(PreprocessSuffix) ccp_react_routine.c

../build-$(ConfigurationName)/CCP/main.c$(ObjectSuffix): main.c ../build-$(ConfigurationName)/CCP/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/main.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/main.c$(DependSuffix) -MM main.c

../build-$(ConfigurationName)/CCP/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/main.c$(PreprocessSuffix) main.c

../build-$(ConfigurationName)/CCP/msgflags.c$(ObjectSuffix): msgflags.c ../build-$(ConfigurationName)/CCP/msgflags.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/msgflags.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/msgflags.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/msgflags.c$(DependSuffix): msgflags.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/msgflags.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/msgflags.c$(DependSuffix) -MM msgflags.c

../build-$(ConfigurationName)/CCP/msgflags.c$(PreprocessSuffix): msgflags.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/msgflags.c$(PreprocessSuffix) msgflags.c

../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(ObjectSuffix): ccp_handle_server.c ../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_handle_server.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_handle_server.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(DependSuffix): ccp_handle_server.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(DependSuffix) -MM ccp_handle_server.c

../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(PreprocessSuffix): ccp_handle_server.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_handle_server.c$(PreprocessSuffix) ccp_handle_server.c

../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(ObjectSuffix): ccp_handle_list.c ../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_handle_list.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_handle_list.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(DependSuffix): ccp_handle_list.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(DependSuffix) -MM ccp_handle_list.c

../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(PreprocessSuffix): ccp_handle_list.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_handle_list.c$(PreprocessSuffix) ccp_handle_list.c

../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(ObjectSuffix): ccp_thread_methods.c ../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_thread_methods.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_thread_methods.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(DependSuffix): ccp_thread_methods.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(DependSuffix) -MM ccp_thread_methods.c

../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(PreprocessSuffix): ccp_thread_methods.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_thread_methods.c$(PreprocessSuffix) ccp_thread_methods.c

../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(ObjectSuffix): ccp_client_routines.c ../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_client_routines.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_client_routines.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(DependSuffix): ccp_client_routines.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(DependSuffix) -MM ccp_client_routines.c

../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(PreprocessSuffix): ccp_client_routines.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_client_routines.c$(PreprocessSuffix) ccp_client_routines.c

../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(ObjectSuffix): ccp_controller_functions.c ../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CCP/ccp_controller_functions.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_controller_functions.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(DependSuffix): ccp_controller_functions.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(DependSuffix) -MM ccp_controller_functions.c

../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(PreprocessSuffix): ccp_controller_functions.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/CCP/ccp_controller_functions.c$(PreprocessSuffix) ccp_controller_functions.c

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


