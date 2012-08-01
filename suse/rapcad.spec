#
# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

Name:           rapcad
Version:        0.8.0
Release:        1
License:        GPLv3+
Summary:        Rapid prototyping CAD IDE for 3D printing machines
Url:            http://rapcad.org/
Group:          Productivity/Graphics/CAD
Source:         http://git.rapcad.org/cgit.cgi/rapcad/snapshot/%{name}-%{version}.tar.bz2
BuildRequires:  flex
BuildRequires:  gcc-c++
BuildRequires:  boost-devel
BuildRequires:  gmp-devel
BuildRequires:  libcgal-devel
BuildRequires:  libdxflib-devel-static
%if 0%{?suse_version}
BuildRequires:  bison
BuildRequires:  libqt4-devel
%endif
%if 0%{?fedora_version}
BuildRequires:  byacc
BuildRequires:  qt-devel
%endif
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
RapCAD is the Rapid prototyping CAD IDE for RepRap and RepStrap 3D printing machines.

%prep
%setup -q

%build
%if 0%{?suse_version}
qmake "CONFIG+=official"
%endif
%if 0%{?fedora_version}
qmake-qt4 "CONFIG+=official"
%endif
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
