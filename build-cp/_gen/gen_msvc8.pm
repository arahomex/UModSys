use strict;

our $generators;
our $script_path;
require "$script_path/gen_msvc_xml.pm";
require "$script_path/gen_msvc_xmlt.pm";

$generators->{'msvc8'} = $generators->{'.msvc_x'}->{'clone'}($generators->{'.msvc_x'}, {
  'templates' => {
    'solution-begin' => <<'EOT',

Microsoft Visual Studio Solution File, Format Version 9.00
# Visual Studio 2005
EOT
    'project-begin' => <<'EOT',
<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="8,00"
	Name="$PROJECT_NAME"
	ProjectGUID="$PROJECT_GUID"
	RootNamespace="$PROJECT_NAME"
	>
EOT
  },
});

return 1;
