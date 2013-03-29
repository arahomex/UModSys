use strict;

our $generators;
our $script_path;
require "$script_path/gen_msvc_xml.pm";

$generators->{'msvc8'} = {
  'templates' => {
    'project-type-c++' => '{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}',
    'solution-begin' => <<'EOT',

Microsoft Visual Studio Solution File, Format Version 9.00
# Visual Studio 2005
EOT
    'solution-project-begin' => <<'EOT',
Project("$PROJECT_TYPE_GUID") = "$PROJECT_NAME", "$PROJECT_PATH", "$PROJECT_GUID"
EOT
    'solution-project-end' => <<'EOT',
EndProject
EOT
    'solution-project-dep-begin' => <<'EOT',
	ProjectSection(ProjectDependencies) = postProject
EOT
    'solution-project-dep-end' => <<'EOT',
	EndProjectSection
EOT
    'solution-project-dep-entry' => <<'EOT',
		$PROJECT_DEP_GUID = $PROJECT_DEP_GUID
EOT
  },
  'open' => sub {
    my ($this) = @_;
    return $this;
  },
  'begins' => {
    'solution' => \&msvc_xml_solution_begin,
  },
  'type' => 'generator',
  'sets' => [{}],
};


q^
  'generator' => sub {
    my ($this, $config) = @_;
    my $template = $this->{'templates'};
    my $projects = {
      'p1' => {
        'name' => 'p1',
        'GUID' => '{AE16F973-229C-446C-9DAD-A19C1A35E6CF}',
        'filename' => 'p1\p1.vcproj',
        'depends' => [],
      },
      'p0' => {
        'name' => 'p0',
        'GUID' => '{ED3D9AA7-91B7-41B1-A1C9-226EACAE0341}',
        'filename' => 'p0\p0.vcproj',
        'depends' => ['p1'],
      }
    };
    generate_msvc_xml_solution('sol.sln', $template, $projects);
  },
^ if 0;

return 1;
