#
# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

Name:           rapcad
Version:        0.9.5
Release:        1
License:        GPLv3+
Summary:        Rapid prototyping CAD IDE for 3D printing machines
Url:            http://rapcad.org/
Group:          Productivity/Graphics/CAD
Source:         http://github.com/GilesBathgate/RapCAD/archive/v%{version}.tar.gz
BuildRequires:  flex
BuildRequires:  gcc-c++
BuildRequires:  boost-devel
BuildRequires:  gmp-devel
BuildRequires:  libcgal-devel
BuildRequires:  readline-devel
BuildRequires:  glu-devel
%if 0%{?suse_version}
BuildRequires:  bison
BuildRequires:  libQt5OpenGL-devel
%endif
%if 0%{?fedora_version}
BuildRequires:  byacc
BuildRequires:  qt5-devel
%endif
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
RapCAD is the Rapid prototyping CAD IDE for RepRap and RepStrap 3D printing machines.

%prep
%setup -q -n RapCAD-%{version}

%build
qmake-qt5 "CONFIG+=official"
make %{?_smp_mflags}

%install
make INSTALL_ROOT=%{buildroot} install

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc COPYING README
%{_bindir}/rapcad

%changelog
