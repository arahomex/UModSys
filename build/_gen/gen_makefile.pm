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
objs__${PROJECT_ID} += \$(tmpx__${PROJECT_ID})/$FNAME.o
\$(tmpx__${PROJECT_ID})/$FNAME.o: $FILE_PATH | \$(tmpx__${PROJECT_ID})
	\@\$(CXX) \$(CXXFLAGS__${PROJECT_ID}) -c -o\$@ \$<
	\@\$(CXX) \$(CXXFLAGS__${PROJECT_ID}) -c -o\$@.d -MM -MT\$@ \$<
-include \$(tmpx__${PROJECT_ID})/$FNAME.o.d
EOT
    'file-c' => <<'EOT',
objs__${PROJECT_ID} += \$(tmpx__${PROJECT_ID})/$FNAME.o
\$(tmpx__${PROJECT_ID})/$FNAME.o: $FILE_PATH | \$(tmpx__${PROJECT_ID})
	\@\$(CC) \$(CFLAGS__${PROJECT_ID}) -c -o\$@ \$<
	\@\$(CC) \$(CXXFLAGS__${PROJECT_ID}) -c -o\$@.d -MM -MT\$@ \$<
-include \$(tmpx__${PROJECT_ID})/$FNAME.o.d
EOT
    'file-S' => <<'EOT',
objs__${PROJECT_ID} += \$(tmpx__${PROJECT_ID})/$FNAME.o
\$(tmpx__${PROJECT_ID})/$FNAME.o: $FILE_PATH | \$(tmpx__${PROJECT_ID})
	\@\$(AS) \$(ASFLAGS__${PROJECT_ID}) -c -o\$@ \$<
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
    'project-tpp-name' => 'tpp-${PROJECTGROUP_NAME}-${N}',
    'project-cpp-name' => 'cpp-${PROJECTGROUP_NAME}-${N}',
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
    'project-depend1t' => '\$(target__${DEPEND1G}__${DEPEND1})',
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
    #
    #
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-config-T:dummy' => <<'EOT',
    'project-config-T:console' => '${OPT_console_OUT}',
    'project-config-T:lib' => '${OPT_lib_OUT}',
    'project-config-T:solib' => '${OPT_solib_OUT}',
    'project-config-T:plugin' => '${OPT_plugin_OUT}',
    'project-config-T:unknown' => '',
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'makefile-dummy' => <<'EOT',
EOT
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
$PROJECT_TNAME: $TARGET_DEPENDS $PROJECT_TPNAME
$PROJECT_CNAME: $CLEAN_DEPENDS $PROJECT_CPNAME
#.PHONY: $PROJECT_TNAME $PROJECT_TPNAME $PROJECT_CNAME $PROJECT_CPNAME
EOT
    'project-config-flags' => <<'EOT',
tmp__${PROJECT_ID}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}
bin__${PROJECT_ID}=${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}
tmpx__${PROJECT_ID}=${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}
CXXFLAGS__${PROJECT_ID}=${OPT_CXXFLAGS}
CFLAGS__${PROJECT_ID}=${OPT_CFLAGS}
LDFLAGS__${PROJECT_ID}=${OPT_LDFLAGS}
EOT
    'project-config-shared' => [
      <<'EOT',
.PHONY: $PROJECTGROUP_TNAME $PROJECTGROUP_CNAME
EOT
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
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-config-M:unknown' => <<'EOT',
# unknown project
EOT
    'project-config-M:dummy' => <<'EOT',
# dummy project
$PROJECT_TPNAME:
$PROJECT_CPNAME:
EOT
    'project-config-M:console' => <<'EOT',
# console binary at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}
$PROJECT_TPNAME: \$(target__${PROJECT_ID})
$PROJECT_CPNAME:
	\@-rm \$(target__${PROJECT_ID})
	\@-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}): $TARGET_DEPENDS \$(objs__${PROJECT_ID}) | \$(bin__${PROJECT_ID}) 
	\@echo '*** Make $MODE $PROJECTGROUP_NAME::$PROJECT_NAME'
	\@\$(CXX) ${OPT_console_OPTIONS} -o\$@ \$(objs__${PROJECT_ID}) \$(LDFLAGS__${PROJECT_ID})
EOT
    'project-config-M:solib' => <<'EOT',
# so library at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so
$PROJECT_TPNAME: \$(target__${PROJECT_ID})
$PROJECT_CPNAME:
	\@-rm \$(target__${PROJECT_ID})
	\@-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}): $TARGET_DEPENDS \$(objs__${PROJECT_ID}) | \$(bin__${PROJECT_ID}) 
	\@echo '*** Make $MODE $PROJECTGROUP_NAME::$PROJECT_NAME'
	\@\$(CXX) ${OPT_solib_OPTIONS} -o\$@ \$(objs__${PROJECT_ID}) \$(LDFLAGS__${PROJECT_ID})
EOT
    'project-config-M:plugin' => <<'EOT',
# so library at ${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so
$PROJECT_TPNAME: \$(target__${PROJECT_ID})
$PROJECT_CPNAME:
	\@-rm \$(target__${PROJECT_ID})
	\@-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}): $TARGET_DEPENDS \$(objs__${PROJECT_ID}) | \$(bin__${PROJECT_ID}) 
	\@echo '*** Make $MODE $PROJECTGROUP_NAME::$PROJECT_NAME'
	\@\$(CXX) ${OPT_plugin_OPTIONS} -o\$@ \$(objs__${PROJECT_ID}) \$(LDFLAGS__${PROJECT_ID})
EOT
    'project-config-M:lib' => <<'EOT',
# library at ${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.a
$PROJECT_TPNAME: \$(target__${PROJECT_ID})
$PROJECT_CPNAME:
	\@-rm \$(target__${PROJECT_ID})
	\@-rm \$(tmpx__${PROJECT_ID})/*
\$(target__${PROJECT_ID}): $TARGET_DEPENDS \$(objs__${PROJECT_ID}) | \$(tmpx__${PROJECT_ID})
	\@echo '*** Make $MODE $PROJECTGROUP_NAME::$PROJECT_NAME'
	\@\$(AR) ${OPT_lib_OPTIONS} \$@ \$(objs__${PROJECT_ID})
EOT
    #----------------------------------------------------------------------------------------------------------------------------
    #----------------------------------------------------------------------------------------------------------------------------
    'project-config-T:dummy' => <<'EOT',
target__${PROJECT_ID}=
EOT
    'project-config-T:console' => <<'EOT',
target__${PROJECT_ID} = ${OPT_console_OUT}
EOT
    'project-config-T:lib' => <<'EOT',
target__${PROJECT_ID}=${OPT_lib_OUT}
EOT
    'project-config-T:solib' => <<'EOT',
target__${PROJECT_ID}=${OPT_solib_OUT}
EOT
    'project-config-T:plugin' => <<'EOT',
target__${PROJECT_ID}=${OPT_plugin_OUT}
EOT
    'project-config-T:unknown' => <<'EOT',
target__${PROJECT_ID}=
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
      'lib_NAME'           => \&option_combiner_last,
      'solib_NAME'         => \&option_combiner_last,
      'plugin_NAME'        => \&option_combiner_last,
      'console_NAME'       => \&option_combiner_last,
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
      'lib_NAME'       => '${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.a',
      'solib_NAME'     => '${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so',
      'plugin_NAME'    => '${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}.so',
      'console_NAME'   => '${PROJECT_NAME}.${PLATFORM_NAME}.${CONF_NAME}',
      #
      'lib_OUT'        => '${PATH_TMP}/${PLATFORM_NAME}.${CONF_NAME}/${OPT_lib_NAME}',
      'solib_OUT'      => '${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${OPT_solib_NAME}',
      'plugin_OUT'     => '${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${OPT_plugin_NAME}',
      'console_OUT'    => '${PATH_BIN}/${PLATFORM_NAME}.${CONF_NAME}/${OPT_console_NAME}',
      #
      'lib_OPTIONS'    => 'r',
      'plugin_OPTIONS' => '-shared -Wl,-rpath,. -Wl,-soname,${OPT_plugin_NAME}',
      'solib_OPTIONS'  => '-shared -Wl,-rpath,. -Wl,-soname,${OPT_solib_NAME}',
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
      'CXXFLAGS'       => '-Os -s',
      'CFLAGS'         => '-Os -s',
      'LDFLAGS'        => '-Os -s',
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
