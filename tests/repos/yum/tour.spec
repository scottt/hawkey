Summary: tour package
Name: tour
Version: 4
Release: 5
Group: Utilities
License: GPLv2+
Distribution: Hawkey test suite.
BuildArch: noarch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-hawkey

%description
Hawkey tour package to test filelists handling.

%prep

%build

%install
mkdir -p %{buildroot}/%_sysconfdir
mkdir -p %{buildroot}/%_bindir
mkdir -p %{buildroot}/%python_sitelib
echo "roll up" > %{buildroot}/%_sysconfdir/rollup
echo "dying to" > %{buildroot}/%_sysconfdir/takeyouaway
echo "take you away" > %{buildroot}/%_bindir/away
echo "take = 3" > %{buildroot}/%python_sitelib/today.py

%files
%_sysconfdir/rollup
%_sysconfdir/takeyouaway
%_bindir/away
%python_sitelib/today.py*
