use strict;

our $generators;
our $script_path;
require "$script_path/gen_msvc_xml.pm";

$generators->{'.msvc_x'} = {
  'templates' => {
    #-------------------------------
    #-------------------------------
    'project-type-c++' => '{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}',
    'solution-begin' => undef,
    #-------------------------------
    'solution-project-begin' => <<'EOT',
Project("$PROJECT_TYPE_GUID") = "$PROJECT_NAME", "$PROJECT_PATH", "$PROJECT_GUID"
EOT
    'solution-project-end' => <<'EOT',
EndProject
EOT
    #-------------------------------
    'solution-project-dep-begin' => <<'EOT',
	ProjectSection(ProjectDependencies) = postProject
EOT
    'solution-project-dep-end' => <<'EOT',
	EndProjectSection
EOT
    'solution-project-dep-entry' => <<'EOT',
		$PROJECT_DEP_GUID = $PROJECT_DEP_GUID
EOT
    #-------------------------------
    #-------------------------------
    'project-begin' => undef,
    'project-end' => <<'EOT',
	<Globals>
	</Globals>
</VisualStudioProject>
EOT
    #-------------------------------
    'project-platforms-begin' => <<'EOT',
	<Platforms>
EOT
    'project-platforms-end' => <<'EOT',
	</Platforms>
EOT
    'project-platforms-entry' => <<'EOT',
		<Platform
			Name="$PLATFORM_NAME"
		/>
EOT
    #-------------------------------
    'project-configs-begin' => <<'EOT',
	<Configurations>
EOT
    'project-configs-end' => <<'EOT',
	</Configurations>
EOT
    'project-config-begin' => <<'EOT',
		<Configuration
			Name="$CONF_NAME|$PLATFORM_NAME"
			OutputDirectory="$OPT_OutputDirectory"
			IntermediateDirectory="$OPT_IntermediateDirectory"
			ConfigurationType="$OPT_ConfigurationType"
			CharacterSet="$OPT_CharacterSet"
			>
EOT
    'project-config-end' => <<'EOT',
		</Configuration>
EOT
    'project-config-aux' => <<'EOT',
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
				OutputFile="$OPT_BscMake_OutputFile"
			/>
			<Tool
				Name="VCALinkTool"
			/>
EOT
    'project-config-compiler' => <<'EOT',
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="$OPT_Compiler_AdditionalOptions"
				Optimization="$OPT_Compiler_Optimization"
				InlineFunctionExpansion="$OPT_Compiler_FavorSizeOrSpeed"
				EnableIntrinsicFunctions="$OPT_Compiler_FavorSizeOrSpeed"
				FavorSizeOrSpeed="$OPT_Compiler_FavorSizeOrSpeed"
				WholeProgramOptimization="$OPT_Compiler_WholeProgramOptimization"
				AdditionalIncludeDirectories="$OPT_Compiler_AdditionalIncludeDirectories"
				PreprocessorDefinitions="$OPT_Compiler_PreprocessorDefinitions"
				StringPooling="$OPT_Compiler_StringPooling"
				MinimalRebuild="$OPT_Compiler_MinimalRebuild"
				BasicRuntimeChecks="$OPT_Compiler_BasicRuntimeChecks"
				RuntimeLibrary="$OPT_Compiler_RuntimeLibrary"
				BufferSecurityCheck="$OPT_Compiler_BufferSecurityCheck"
				RuntimeTypeInfo="$OPT_Compiler_RuntimeTypeInfo"
				UsePrecompiledHeader="$OPT_Compiler_UsePrecompiledHeader"
				PrecompiledHeaderFile="$OPT_Compiler_PrecompiledHeaderFile"
				PrecompiledHeaderThrough="$OPT_Compiler_PrecompiledHeaderThrough"
				AssemblerListingLocation="$OPT_Compiler_AssemblerListingLocation"
				ProgramDataBaseFileName="$OPT_Compiler_ProgramDataBaseFileName"
				BrowseInformation="$OPT_Compiler_BrowseInformation"
				WarningLevel="$OPT_Compiler_WarningLevel"
				WarnAsError="$OPT_Compiler_WarnAsError"
				Detect64BitPortabilityProblems="$OPT_Compiler_Detect64BitPortabilityProblems"
				DebugInformationFormat="$OPT_Compiler_DebugInformationFormat"
				CompileAs="$OPT_Compiler_CompileAs"
				EnablePREfast="$OPT_Compiler_EnablePREfast"
			/>
EOT
    'project-config-linker' => <<'EOT',
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="$OPT_Linker_AdditionalDependencies"
				OutputFile="$OPT_Linker_OutputFile"
				LinkIncremental="$OPT_Linker_LinkIncremental"
				AdditionalLibraryDirectories="$OPT_Linker_AdditionalLibraryDirectories"
				GenerateDebugInformation="$OPT_Linker_GenerateDebugInformation"
				ProgramDatabaseFile="$OPT_Linker_ProgramDatabaseFile"
				GenerateMapFile="$OPT_Linker_GenerateMapFile"
				MapFileName="$OPT_Linker_MapFileName"
				MapExports="$OPT_Linker_MapExports"
				SubSystem="$OPT_Linker_SubSystem"
				LinkTimeCodeGeneration="$OPT_Linker_LinkTimeCodeGeneration"
			/>
EOT
    'project-config-librarian' => <<'EOT',
			<Tool
				Name="VCLibrarianTool"
				OutputFile="$OPT_Librarian_OutputFile"
			/>
EOT
    #-------------------------------
    'project-files-begin' => <<'EOT',
	<Files>
EOT
    'project-files-end' => <<'EOT',
	</Files>
EOT
    #-------------------------------
    'project-ff-begin' => <<'EOT',
$FF_PAD		<Filter
$FF_PAD			Name="$FILTER_NAME"
$FF_PAD			>
EOT
    'project-ff-end' => <<'EOT',
$FF_PAD		</Filter>
EOT
    'project-ff-file-begin' => <<'EOT',
$FF_PAD			<File
$FF_PAD				RelativePath="$FILE_PATH"
$FF_PAD				>
EOT
    'project-ff-file-end' => <<'EOT',
$FF_PAD			</File>
EOT
    'project-ff-file-xopt-begin' => <<'EOT',
$FF_PAD				<FileConfiguration
$FF_PAD					Name="$CONF_NAME|$PLATFORM_NAME"
$FF_PAD					>
EOT
    'project-ff-file-xopt-end' => <<'EOT',
$FF_PAD				</FileConfiguration>
EOT
    'project-ff-file-xopt-tbegin-Compiler' => <<'EOT',
$FF_PAD					<Tool
$FF_PAD						Name="VCCLCompilerTool"
EOT
    'project-ff-file-xopt-tend' => <<'EOT',
$FF_PAD					/>
EOT
    'project-ff-file-xopt-tvalue' => <<'EOT',
$FF_PAD						$KEY="$VALUE"
EOT
    #-------------------------------
  },
  #-------------------------------
  #-------------------------------
  'a-project-opts' => {
    '[]' => {
      'Platforms'      => 'Win32 X64 IA64',
      'Configurations' => 'Debug Release ReleaseSpace ReleaseStatic',
      #
      '#' => {
        'Defines'                                   => '',
        'OutputDirectory'                           => '',
        'IntermediateDirectory'                     => '',
        'ConfigurationType'                         => '',
        'ConfigurationType'                         => '',
        'CharacterSet'                              => '',
        #-------------------------------
        'Compiler_Optimization'                     => '',
        'Compiler_InlineFunctionExpansion'          => '',
        'Compiler_EnableIntrinsicFunctions'         => '',
        'Compiler_FavorSizeOrSpeed'                 => '',
        'Compiler_WholeProgramOptimization'         => '',
        'Compiler_AdditionalIncludeDirectories'     => '',
        'Compiler_PreprocessorDefinitions'          => '',
        'Compiler_StringPooling'                    => '',
        'Compiler_MinimalRebuild'                   => '',
        'Compiler_BasicRuntimeChecks'               => '',
	'Compiler_BufferSecurityCheck'              => '',
        'Compiler_RuntimeLibrary'                   => '',
        'Compiler_RuntimeTypeInfo'                  => '',
        'Compiler_UsePrecompiledHeader'             => '',
        'Compiler_PrecompiledHeaderFile'            => '',
        'Compiler_PrecompiledHeaderThrough'         => '',
        'Compiler_AssemblerListingLocation'         => '',
        'Compiler_ProgramDataBaseFileName'          => '',
        'Compiler_BrowseInformation'                => '',
        'Compiler_WarningLevel'                     => '',
        'Compiler_WarnAsError'                      => '',
        'Compiler_Detect64BitPortabilityProblems'   => '',
        'Compiler_DebugInformationFormat'           => '',
        'Compiler_CompileAs'                        => '',
        'Compiler_EnablePREfast'                    => '',
        #-------------------------------
        'Linker_AdditionalDependencies'             => '',
        'Linker_OutputFile'                         => '',
        'Linker_LinkIncremental'                    => '',
        'Linker_AdditionalLibraryDirectories'       => '',
        'Linker_GenerateDebugInformation'           => '',
        'Linker_ProgramDatabaseFile'                => '',
        'Linker_GenerateMapFile'                    => '',
        'Linker_MapFileName'                        => '',
        'Linker_MapExports'                         => '',
        'Linker_SubSystem'                          => '',
        'Linker_LinkTimeCodeGeneration'             => '',
        #-------------------------------
        'Librarian_OutputFile'                      => '',
        #
        'BscMake_OutputFile'                        => '',
      },
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
      #
      'BscMake_OutputFile'                          => '$(IntDir)/$(ProjectName).bsc',
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
    'project-group' => \&msvc_xml_solution_begin,
  },
  'set' => \&set_value,
  'commands' => {
    'include' => \&msvc_xml_gen_include,
  },
  'generate' => \&msvc_xml_gen_generate,
  #-------------------------------
  'clone' => \&msvc_xml_clone,
  'subs' => {},
};


return 1;
