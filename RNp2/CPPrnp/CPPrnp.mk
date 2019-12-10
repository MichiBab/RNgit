##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=CPPrnp
ConfigurationName      :=Debug
WorkspacePath          :=/home/michi/OneDrive/RNgit/RNp2
ProjectPath            :=/home/michi/OneDrive/RNgit/RNp2/CPPrnp
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Michi
Date                   :=10/12/19
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="CPPrnp.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
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
Objects0=$(IntermediateDirectory)/server.c$(ObjectSuffix) $(IntermediateDirectory)/msgflags.c$(ObjectSuffix) $(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/client.c$(ObjectSuffix) $(IntermediateDirectory)/ccp_thread_methods.c$(ObjectSuffix) $(IntermediateDirectory)/ccp_react_routine.c$(ObjectSuffix) $(IntermediateDirectory)/ccp_handle_list.c$(ObjectSuffix) $(IntermediateDirectory)/ccp_create_packs.c$(ObjectSuffix) $(IntermediateDirectory)/ccp_handle_server.c$(ObjectSuffix) $(IntermediateDirectory)/ccp_controller_functions.c$(ObjectSuffix) \
	$(IntermediateDirectory)/ccp_client_routines.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/server.c$(ObjectSuffix): server.c $(IntermediateDirectory)/server.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/server.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/server.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/server.c$(DependSuffix): server.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/server.c$(ObjectSuffix) -MF$(IntermediateDirectory)/server.c$(DependSuffix) -MM server.c

$(IntermediateDirectory)/server.c$(PreprocessSuffix): server.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/server.c$(PreprocessSuffix) server.c

$(IntermediateDirectory)/msgflags.c$(ObjectSuffix): msgflags.c $(IntermediateDirectory)/msgflags.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/msgflags.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/msgflags.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/msgflags.c$(DependSuffix): msgflags.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/msgflags.c$(ObjectSuffix) -MF$(IntermediateDirectory)/msgflags.c$(DependSuffix) -MM msgflags.c

$(IntermediateDirectory)/msgflags.c$(PreprocessSuffix): msgflags.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/msgflags.c$(PreprocessSuffix) msgflags.c

$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

$(IntermediateDirectory)/client.c$(ObjectSuffix): client.c $(IntermediateDirectory)/client.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/client.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/client.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/client.c$(DependSuffix): client.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/client.c$(ObjectSuffix) -MF$(IntermediateDirectory)/client.c$(DependSuffix) -MM client.c

$(IntermediateDirectory)/client.c$(PreprocessSuffix): client.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/client.c$(PreprocessSuffix) client.c

$(IntermediateDirectory)/ccp_thread_methods.c$(ObjectSuffix): ccp_thread_methods.c $(IntermediateDirectory)/ccp_thread_methods.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_thread_methods.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_thread_methods.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_thread_methods.c$(DependSuffix): ccp_thread_methods.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_thread_methods.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_thread_methods.c$(DependSuffix) -MM ccp_thread_methods.c

$(IntermediateDirectory)/ccp_thread_methods.c$(PreprocessSuffix): ccp_thread_methods.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_thread_methods.c$(PreprocessSuffix) ccp_thread_methods.c

$(IntermediateDirectory)/ccp_react_routine.c$(ObjectSuffix): ccp_react_routine.c $(IntermediateDirectory)/ccp_react_routine.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_react_routine.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_react_routine.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_react_routine.c$(DependSuffix): ccp_react_routine.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_react_routine.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_react_routine.c$(DependSuffix) -MM ccp_react_routine.c

$(IntermediateDirectory)/ccp_react_routine.c$(PreprocessSuffix): ccp_react_routine.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_react_routine.c$(PreprocessSuffix) ccp_react_routine.c

$(IntermediateDirectory)/ccp_handle_list.c$(ObjectSuffix): ccp_handle_list.c $(IntermediateDirectory)/ccp_handle_list.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_handle_list.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_handle_list.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_handle_list.c$(DependSuffix): ccp_handle_list.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_handle_list.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_handle_list.c$(DependSuffix) -MM ccp_handle_list.c

$(IntermediateDirectory)/ccp_handle_list.c$(PreprocessSuffix): ccp_handle_list.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_handle_list.c$(PreprocessSuffix) ccp_handle_list.c

$(IntermediateDirectory)/ccp_create_packs.c$(ObjectSuffix): ccp_create_packs.c $(IntermediateDirectory)/ccp_create_packs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_create_packs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_create_packs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_create_packs.c$(DependSuffix): ccp_create_packs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_create_packs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_create_packs.c$(DependSuffix) -MM ccp_create_packs.c

$(IntermediateDirectory)/ccp_create_packs.c$(PreprocessSuffix): ccp_create_packs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_create_packs.c$(PreprocessSuffix) ccp_create_packs.c

$(IntermediateDirectory)/ccp_handle_server.c$(ObjectSuffix): ccp_handle_server.c $(IntermediateDirectory)/ccp_handle_server.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_handle_server.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_handle_server.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_handle_server.c$(DependSuffix): ccp_handle_server.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_handle_server.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_handle_server.c$(DependSuffix) -MM ccp_handle_server.c

$(IntermediateDirectory)/ccp_handle_server.c$(PreprocessSuffix): ccp_handle_server.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_handle_server.c$(PreprocessSuffix) ccp_handle_server.c

$(IntermediateDirectory)/ccp_controller_functions.c$(ObjectSuffix): ccp_controller_functions.c $(IntermediateDirectory)/ccp_controller_functions.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_controller_functions.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_controller_functions.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_controller_functions.c$(DependSuffix): ccp_controller_functions.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_controller_functions.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_controller_functions.c$(DependSuffix) -MM ccp_controller_functions.c

$(IntermediateDirectory)/ccp_controller_functions.c$(PreprocessSuffix): ccp_controller_functions.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_controller_functions.c$(PreprocessSuffix) ccp_controller_functions.c

$(IntermediateDirectory)/ccp_client_routines.c$(ObjectSuffix): ccp_client_routines.c $(IntermediateDirectory)/ccp_client_routines.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/michi/OneDrive/RNgit/RNp2/CPPrnp/ccp_client_routines.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ccp_client_routines.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ccp_client_routines.c$(DependSuffix): ccp_client_routines.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ccp_client_routines.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ccp_client_routines.c$(DependSuffix) -MM ccp_client_routines.c

$(IntermediateDirectory)/ccp_client_routines.c$(PreprocessSuffix): ccp_client_routines.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ccp_client_routines.c$(PreprocessSuffix) ccp_client_routines.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


