use strict;
use warnings;

our $generators;
our $script_path;
require "$script_path/gen_makefile_func.pm";

$generators->{'makefile'} = {
  'templates' => {
    #-------------------------------
    'project-tp-name' => 'tp-${PROJECTGROUP_NAME}-${N}-${P}-${C}',
    'project-cp-name' => 'cp-${PROJECTGROUP_NAME}-${N}-${P}-${C}',
    'project-tg-name' => 'tg-${PROJECTGROUP_NAME}-${P}-${C}',
    'project-cg-name' => 'cg-${PROJECTGROUP_NAME}-${P}-${C}',
    'project-tt-name' => 'target-${P}-${C}',
    'project-cc-name' => 'clean-${P}-${C}',
    #-------------------------------
    'makefile-begin' => <<'EOT',
################################################################# BEGIN makefile

.PHONY: all target clean

all: target

EOT
    'makefile-end' => <<'EOT',
################################################################# END makefile
EOT

    'projectgroup-begin' => <<'EOT',
#**************************************************************** BEGIN projectgroup '$PROJECTGROUP_NAME'

EOT
    'projectgroup-end' => <<'EOT',

#**************************************************************** EPILOG '$PROJECTGROUP_NAME'
#**************************************************************** END projectgroup '$PROJECTGROUP_NAME'
EOT
    'project-begin' => <<'EOT',
#---------------------------------------------------------------- BEGIN project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' 
#-------------------------------- prolog '$PROJECTGROUP_NAME'.'$PROJECT_NAME'
#--------------------------------


EOT
    'project-end' => <<'EOT',
#-------------------------------- epilog '$PROJECTGROUP_NAME'.'$PROJECT_NAME'
#---------------------------------------------------------------- END project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' 

EOT
    'project-config-begin' => <<'EOT',
#-------------------------------- project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' PLATFORM=${PLATFORM_NAME} CONFIG=${CONF_NAME} MODE=$MODE:
$TARGET_TNAME: $PROJECT_TNAME
$TARGET_CNAME: $PROJECT_CNAME
$PROJECTGROUP_TNAME: $PROJECT_TNAME
$PROJECTGROUP_CNAME: $PROJECT_CNAME
$PROJECT_TNAME: $TARGET_DEPENDS
$PROJECT_CNAME: $CLEAN_DEPENDS

tmp__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}
bin__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}
tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}
EOT
    'project-config-end' => <<'EOT',
#-------------------------------- /project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' PLATFORM=${PLATFORM_NAME} CONFIG=${CONF_NAME} MODE=$MODE
EOT
    'project-config-M-general' => <<'EOT',
EOT
    'project-config-M:dummy' => <<'EOT',
# dummy project
target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=
EOT
    'project-config-M:console' => <<'EOT',
# console binary at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}
target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}
$PROJECT_TNAME: \\
  \$(tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}) \\
  \$(bin__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}) \\
  \$(target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})
$PROJECT_CNAME:
	-rm \$(target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})
	-rm \$(tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})/*
EOT
    'project-config-M:lib' => <<'EOT',
# library at ${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.a
target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.a
$PROJECT_TNAME: \\
  \$(tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}) \\
  \$(target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})
$PROJECT_CNAME:
	-rm \$(target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})
	-rm \$(tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})/*
EOT
    'project-config-M:solib' => <<'EOT',
# so library at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so
target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}=${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so
$PROJECT_TNAME: \\
  \$(tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}) \\
  \$(bin__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME}) \\
  \$(target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})
$PROJECT_CNAME:
	-rm \$(target__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})
	-rm \$(tmpx__${PROJECTGROUP_NAME}__${PROJECT_NAME}__${PLATFORM_NAME}__${CONF_NAME})/*
EOT
    #-------------------------------
  },
  #-------------------------------
  #-------------------------------
  'a-project-opts' => {
    '[]' => {
      'Platforms'      => 'Linux_x86_64 Linux_i686',
      'Configurations' => 'Debug Release ReleaseSpace ReleaseStatic',
    },
    #
    '*' => {
      'OutputDirectory'                             => '$(SolutionDir)../../tmp/win32vc8_$(ConfigurationName)_$(PlatformName)',
      'IntermediateDirectory'                       => '$(SolutionDir)../../tmp/win32vc8_$(ConfigurationName)_$(PlatformName)/$(ProjectName)',
      'CharacterSet'                                => '2',
      #                                            
      'Compiler_AdditionalOptions'                  => '/MP',
      'Compiler_AdditionalIncludeDirectories'       => '$(SolutionDir)../../include',
      'Compiler_StringPooling'                      => 'true',
      'Compiler_MinimalRebuild'                     => 'false',
      'Compiler_RuntimeTypeInfo'                    => 'true',
      'Compiler_UsePrecompiledHeader'               => '0',
      'Compiler_PrecompiledHeaderFile'              => '$(IntDir)/$(ProjectName).pch',
      'Compiler_PrecompiledHeaderThrough'           => 'StdAfx.h',
      'Compiler_AssemblerListingLocation'           => '$(IntDir)/',
      'Compiler_ProgramDataBaseFileName'            => '$(IntDir)/',
      'Compiler_Detect64BitPortabilityProblems'     => 'true',
      'Compiler_CompileAs'                          => '0',
      'Compiler_EnablePREfast'                      => 'false',
      #
      'Librarian_OutputFile'                        => '$(OutDir)/$(ProjectName).lib',
      #
      'Linker_AdditionalDependencies'               => 'Winmm.lib ws2_32.lib',
      'Linker_OutputFile'                           => '$(OutDir)/$(ProjectName).exe',
      'Linker_AdditionalLibraryDirectories'         => '$(IntDir)/..',
      'Linker_ProgramDatabaseFile'                  => '$(OutDir)/$(ProjectName).pdb',
      'Linker_MapFileName'                          => '$(IntDir)/$(ProjectName).map',
      'Linker_MapExports'                           => 'true',
      'Linker_SubSystem'                            => '1',
      'Linker_TargetMachine'                        => '',
      #
      'BscMake_OutputFile'                          => '$(IntDir)/$(ProjectName).bsc',
      #
      'PreBuild_Description'                        => 'Pre Build',
      'PreBuild_CommandLine'                        => '',
      'PreBuild_Excluded'                           => 'false',
      #
      'PreLink_Description'                         => 'Pre Link',
      'PreLink_CommandLine'                         => '',
      'PreLink_Excluded'                            => 'false',
      #
      'PostBuild_Description'                       => 'Post Build',
      'PostBuild_CommandLine'                       => '',
      'PostBuild_Excluded'                          => 'false',
      #
      'CustomBuild_Description'                     => 'Custom Build',
      'CustomBuild_CommandLine'                     => '',
      'CustomBuild_AdditionalDependencies'          => '',
      'CustomBuild_Outputs'                         => '',
    },
    'Debug' => {
      'Compiler_Optimization'                       => '0',
      'Compiler_InlineFunctionExpansion'            => '0',
      'Compiler_EnableIntrinsicFunctions'           => 'false',
      'Compiler_FavorSizeOrSpeed'                   => '0',
      'Compiler_WholeProgramOptimization'           => 'false',
      'Compiler_PreprocessorDefinitions'            => 'WIN32;_DEBUG',
      'Compiler_BasicRuntimeChecks'                 => '3',
      'Compiler_BufferSecurityCheck'                => 'true',
      'Compiler_RuntimeLibrary'                     => '3',
      'Compiler_BrowseInformation'                  => '1',
      'Compiler_WarningLevel'                       => '3',
      'Compiler_WarnAsError'                        => 'true',
      'Compiler_DebugInformationFormat'             => '3',
      #
      'Linker_LinkIncremental'                      => '1',
      'Linker_GenerateDebugInformation'             => 'true',
      'Linker_GenerateMapFile'                      => 'true',
      'Linker_LinkTimeCodeGeneration'               => '0',
    },
    'Release' => {
      'Compiler_Optimization'                       => '3',
      'Compiler_InlineFunctionExpansion'            => '2',
      'Compiler_EnableIntrinsicFunctions'           => 'true',
      'Compiler_FavorSizeOrSpeed'                   => '1',
      'Compiler_WholeProgramOptimization'           => 'true',
      'Compiler_PreprocessorDefinitions'            => 'WIN32;NDEBUG',
      'Compiler_BasicRuntimeChecks'                 => '0',
      'Compiler_BufferSecurityCheck'                => 'false',
      'Compiler_RuntimeLibrary'                     => '2',
      'Compiler_BrowseInformation'                  => '0',
      'Compiler_WarningLevel'                       => '1',
      'Compiler_WarnAsError'                        => 'true',
      'Compiler_DebugInformationFormat'             => '0',
      #
      'Linker_LinkIncremental'                      => '0',
      'Linker_GenerateDebugInformation'             => 'false',
      'Linker_GenerateMapFile'                      => 'false',
      'Linker_LinkTimeCodeGeneration'               => '1',
    },
    'ReleaseSpace' => {
      'Compiler_Optimization'                       => '1',
      'Compiler_InlineFunctionExpansion'            => '0',
      'Compiler_EnableIntrinsicFunctions'           => 'false',
      'Compiler_FavorSizeOrSpeed'                   => '2',
      'Compiler_WholeProgramOptimization'           => 'true',
      'Compiler_PreprocessorDefinitions'            => 'WIN32;NDEBUG',
      'Compiler_BasicRuntimeChecks'                 => '0',
      'Compiler_BufferSecurityCheck'                => 'false',
      'Compiler_RuntimeLibrary'                     => '2',
      'Compiler_BrowseInformation'                  => '0',
      'Compiler_WarningLevel'                       => '1',
      'Compiler_WarnAsError'                        => 'true',
      'Compiler_DebugInformationFormat'             => '0',
      #
      'Linker_LinkIncremental'                      => '0',
      'Linker_GenerateDebugInformation'             => 'false',
      'Linker_GenerateMapFile'                      => 'false',
      'Linker_LinkTimeCodeGeneration'               => '1',
    },
    'ReleaseStatic' => {
      'Compiler_Optimization'                       => '3',
      'Compiler_InlineFunctionExpansion'            => '2',
      'Compiler_EnableIntrinsicFunctions'           => 'true',
      'Compiler_FavorSizeOrSpeed'                   => '1',
      'Compiler_WholeProgramOptimization'           => 'true',
      'Compiler_PreprocessorDefinitions'            => 'WIN32;NDEBUG',
      'Compiler_BasicRuntimeChecks'                 => '0',
      'Compiler_BufferSecurityCheck'                => 'false',
      'Compiler_RuntimeLibrary'                     => '0',
      'Compiler_BrowseInformation'                  => '0',
      'Compiler_WarningLevel'                       => '1',
      'Compiler_WarnAsError'                        => 'true',
      'Compiler_DebugInformationFormat'             => '0',
      #
      'Linker_LinkIncremental'                      => '0',
      'Linker_GenerateDebugInformation'             => 'false',
      'Linker_GenerateMapFile'                      => 'false',
      'Linker_LinkTimeCodeGeneration'               => '1',
    },
  },
  #-------------------------------
  #-------------------------------
  #-------------------------------
  #-------------------------------
  'type' => 'generator',
  'sets' => [{}],
  #
  'open' => sub {
    my ($this) = @_;
    return $this;
  },
  'begins' => {
    'project-group' => \&makefile_projectgroup_begin,
  },
  'set' => \&set_value,
  'commands' => {
    'include' => \&makefile_gen_include,
  },
  'generate' => \&makefile_gen_generate,
  #-------------------------------
  'subs' => {},
};


return 1;
