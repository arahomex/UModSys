use strict;
use warnings;

our $generators;

#our $script_path;
#require "$script_path/gen_makefile_func.pm";
#our $fout;

$generators->{'makefile'} = {
  'templates' => {
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'file-cpp' => <<'EOT',
\$(tmpx__${PROJECT_ID})/$FNAME.o: $FILE_PATH
	\$(CXX) \$(CXXFLAGS__${PROJECT_ID}) -c -o\$@ \$<
	\$(CXX) \$(CXXFLAGS__${PROJECT_ID}) -c -o\$@.d -MM -MT\$@ \$<
-include \$(tmpx__${PROJECT_ID})/$FNAME.o.d
\$(target__${PROJECT_ID}): \$(tmpx__${PROJECT_ID})/$FNAME.o
EOT
    'file-c' => <<'EOT',
\$(tmpx__${PROJECT_ID})/$FNAME.o: $FILE_PATH
	\$(CC) \$(CFLAGS__${PROJECT_ID}) -c -o\$@ \$<
	\$(CC) \$(CXXFLAGS__${PROJECT_ID}) -c -o\$@.d -MM -MT\$@ \$<
-include \$(tmpx__${PROJECT_ID})/$FNAME.o.d
\$(target__${PROJECT_ID}): \$(tmpx__${PROJECT_ID})/$FNAME.o
EOT
    'file-S' => <<'EOT',
\$(tmpx__${PROJECT_ID})/$FNAME.o: $FILE_PATH
	\$(AS) \$(ASFLAGS__${PROJECT_ID}) -c -o\$@ \$<
\$(target__${PROJECT_ID}): \$(tmpx__${PROJECT_ID})/$FNAME.o
EOT
    #
    'file_generators' => {
      '.cpp' => \&makefile_genfile_cpp,
      '.c' => \&makefile_genfile_c,
      '.S' => \&makefile_genfile_S,
      '.s' => \&makefile_genfile_S,
    },
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-tp-name' => 'tp-${PROJECTGROUP_NAME}-${N}',
    'project-cp-name' => 'cp-${PROJECTGROUP_NAME}-${N}',
    'project-tg-name' => 'tg-${PROJECTGROUP_NAME}',
    'project-cg-name' => 'cg-${PROJECTGROUP_NAME}',
    'project-tt-name' => 'target',
    'project-cc-name' => 'clean',
    'project-id-name' => '${PROJECTGROUP_NAME}__${PROJECT_NAME}',
    #
    'project-include1' => ' -I${INCLUDE1}',
    'project-define1' => ' -D${DEFINE1}',
    'project-lib1' => ' -l${LIB1}',
    'project-libpath1' => ' -L${LIBPATH1}',
    #
    'project-depend1t' => 'tp-${DEPEND1G}-${DEPEND1}',
    'project-depend1c' => 'cp-${DEPEND1G}-${DEPEND1}',
    #
    'project-depend1C:dummy' => '',
    'project-depend1C:console' => '',
    'project-depend1C:lib' => '',
    'project-depend1C:solib' => '',
    'project-depend1C:plugin' => '',
    'project-depend1C:unknown' => '',
    #
    'project-depend1L:dummy' => '',
    'project-depend1L:console' => '',
    'project-depend1L:lib' => ' -l:${DEPEND1}.${PLATFORM_NAME}.${CONF_NAME}.a',
    'project-depend1L:solib' => ' -l:${DEPEND1}.${PLATFORM_NAME}.${CONF_NAME}.so',
    'project-depend1L:plugin' => '',
    'project-depend1L:unknown' => '',
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'makefile-begin' => <<'EOT',
################################################################# BEGIN makefile

.PHONY: all target clean

all: target

EOT
    'makefile-xmap' => <<'EOT',
################################################################# XMAP makefile
EOT
    'makefile-end' => <<'EOT',
################################################################# END makefile
EOT
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'projectgroup-begin' => <<'EOT',
#**************************************************************** BEGIN projectgroup '$PROJECTGROUP_NAME'

EOT
    'projectgroup-end' => <<'EOT',

#**************************************************************** EPILOG '$PROJECTGROUP_NAME'
#**************************************************************** END projectgroup '$PROJECTGROUP_NAME'
EOT
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-begin' => <<'EOT',
#---------------------------------------------------------------- BEGIN project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' 
#-------------------------------- prolog '$PROJECTGROUP_NAME'.'$PROJECT_NAME'
#--------------------------------


EOT
    'project-end' => <<'EOT',
#-------------------------------- epilog '$PROJECTGROUP_NAME'.'$PROJECT_NAME'
#---------------------------------------------------------------- END project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' 

EOT
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-config-begin' => <<'EOT',
#-------------------------------- project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' PLATFORM=${PLATFORM_NAME} CONFIG=${CONF_NAME} MODE=$MODE:
$TARGET_TNAME: $PROJECT_TNAME
$TARGET_CNAME: $PROJECT_CNAME
$PROJECTGROUP_TNAME: $PROJECT_TNAME
$PROJECTGROUP_CNAME: $PROJECT_CNAME
$PROJECT_TNAME: $TARGET_DEPENDS
$PROJECT_CNAME: $CLEAN_DEPENDS

tmp__${PROJECT_ID}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}
bin__${PROJECT_ID}=${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}
tmpx__${PROJECT_ID}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}
CXXFLAGS__${PROJECT_ID}=${OPT_CXXFLAGS}
CFLAGS__${PROJECT_ID}=${OPT_CFLAGS}
LDFLAGS__${PROJECT_ID}=${OPT_LDFLAGS}
EOT
    'project-config-shared' => [
      <<'EOT',
${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}:
	mkdir -p \$@
EOT
      <<'EOT',
${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}:
	mkdir -p \$@
EOT
      <<'EOT',
${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}: | ${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}
	mkdir -p \$@
EOT
    ],
    'project-config-end' => <<'EOT',
${PROJECT_CONTENTS}
#-------------------------------- /project '$PROJECTGROUP_NAME'.'$PROJECT_NAME' PLATFORM=${PLATFORM_NAME} CONFIG=${CONF_NAME} MODE=$MODE
EOT
    'project-config-M-general' => <<'EOT',
EOT
    'project-config-M:unknown' => <<'EOT',
# dummy project
target__${PROJECT_ID}=
EOT
    'project-config-M:dummy' => <<'EOT',
# dummy project
target__${PROJECT_ID}=
EOT
    'project-config-M:console' => <<'EOT',
# console binary at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}
target__${PROJECT_ID} = ${OPT_console_OUT}
$PROJECT_TNAME: \$(tmpx__${PROJECT_ID}) \$(bin__${PROJECT_ID}) \$(target__${PROJECT_ID})
$PROJECT_CNAME:
	-rm \$(target__${PROJECT_ID})
	-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}):
	\$(CXX) ${OPT_console_OPTIONS} -o\$@ \$+ \$(LDFLAGS__${PROJECT_ID})
EOT
    'project-config-M:solib' => <<'EOT',
# so library at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so
target__${PROJECT_ID}=${OPT_solib_OUT}
$PROJECT_TNAME: \$(tmpx__${PROJECT_ID}) \$(bin__${PROJECT_ID}) \$(target__${PROJECT_ID})
$PROJECT_CNAME:
	-rm \$(target__${PROJECT_ID})
	-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}):
	\$(CXX) ${OPT_solib_OPTIONS} -o\$@ \$+ \$(LDFLAGS__${PROJECT_ID})
EOT
    'project-config-M:plugin' => <<'EOT',
# so library at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so
target__${PROJECT_ID}=${OPT_plugin_OUT}
$PROJECT_TNAME: \$(tmpx__${PROJECT_ID}) \$(bin__${PROJECT_ID}) \$(target__${PROJECT_ID})
$PROJECT_CNAME:
	-rm \$(target__${PROJECT_ID})
	-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}):
        
	\$(CXX) ${OPT_plugin_OPTIONS} -o\$@ \$+ \$(LDFLAGS__${PROJECT_ID})
EOT
    'project-config-M:lib' => <<'EOT',
# library at ${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.a
target__${PROJECT_ID}=${OPT_lib_OUT}
$PROJECT_TNAME: \$(tmpx__${PROJECT_ID}) \$(target__${PROJECT_ID})
$PROJECT_CNAME:
	-rm \$(target__${PROJECT_ID})
	-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}):
	\$(AR) ${OPT_lib_OPTIONS} \$@ \$?
EOT
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-ff-begin' => <<'EOT',
# file group $FILEGROUP_NAME :
EOT
    'project-ff-end' => <<'EOT',
# end file group $FILEGROUP_NAME .
EOT
    'project-ff-file-begin' => <<'EOT',
# file $FILEGROUP_NAME : $FILE_PATH
EOT
    'project-ff-file-end' => <<'EOT',
EOT
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    '#options' => {
      'CFLAGS'             => \&option_combiner_space,
      'CXXFLAGS'           => \&option_combiner_space,
      'LDFLAGS'            => \&option_combiner_space,
      #
      'lib_OUT'            => \&option_combiner_last,
      'solib_OUT'          => \&option_combiner_last,
      'plugin_OUT'         => \&option_combiner_last,
      'console_OUT'        => \&option_combiner_last,
      #
      'lib_OPTIONS'        => \&option_combiner_last,
      'solib_OPTIONS'      => \&option_combiner_last,
      'plugin_OPTIONS'     => \&option_combiner_last,
      'console_OPTIONS'    => \&option_combiner_last,
    },
  },
  #-------------------------------
  #-------------------------------
  #-------------------------------
  #-------------------------------
  'a-project-opts' => {
    '[]' => {
      'Platforms'      => $ENV{'PLATFORMS'}, #'Linux_x86_64 Linux_i686',
      'Configurations' => 'Debug Release ReleaseSpace',
    },
    #
    '*' => {
      'lib_OUT'        => '${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.a',
      'solib_OUT'      => '${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so',
      'plugin_OUT'     => '${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so',
      'console_OUT'    => '${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}',
      #
      'lib_OPTIONS'    => 'r',
      'plugin_OPTIONS' => '-shared -Wl,-rpath,. -Wl,-soname,${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so',
      'solib_OPTIONS'  => '-shared -Wl,-rpath,. -Wl,-soname,${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so',
      'console_OPTIONS'=> '-Wl,-rpath,.',
      #
      'CXXFLAGS'       => '-fPIC',
      'CFLAGS'         => '-fPIC',
      'LDFLAGS'        => '-fPIC '
                         .'-Wl,-z,defs -Wl,-rpath-link,${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}'
                         .' -L${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME} -L${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}'
                         .' -ldl -lpthread',
    },
    'Debug' => {
      'CXXFLAGS'       => '-O0 -g',
      'CFLAGS'         => '-O0 -g',
      'LDFLAGS'        => '-O0 -g',
    },
    'Release' => {
      'CXXFLAGS'       => '-O3',
      'CFLAGS'         => '-O3',
      'LDFLAGS'        => '-O3',
    },
    'ReleaseSpace'     => {
      'CXXFLAGS'       => '-Os',
      'CFLAGS'         => '-Os',
      'LDFLAGS'        => '-Os',
    },
  },
  #-------------------------------
  #-------------------------------
  #-------------------------------
  #-------------------------------
  'type' => 'generator',
  'sets' => [{}],
  #
  'open' => \&makefile_open,
  #
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
  'pc-targets' => {
    'platforms' => {},
    'configs' => {},
    'targets' => {},
  },
};


return 1;
