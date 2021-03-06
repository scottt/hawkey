import sys
import unittest

import base
import hawkey
import hawkey.test

class Sanity(base.TestCase):
    def test_sanity(self):
        assert(self.repo_dir)
        sack = hawkey.test.TestSack(repo_dir=self.repo_dir)
        self.assertEqual(sack.nsolvables, 2)

    def test_load_rpm(self):
        sack = hawkey.test.TestSack(repo_dir=self.repo_dir)
        sack.load_rpm_repo()
        self.assertEqual(sack.nsolvables, hawkey.test.EXPECT_SYSTEM_NSOLVABLES)

    def test_load_yum(self):
        sack = hawkey.test.TestSack(repo_dir=self.repo_dir)
        sack.load_rpm_repo()
        sack.load_yum_repo()
        self.assertEqual(sack.nsolvables, hawkey.test.EXPECT_YUM_NSOLVABLES +
                         hawkey.test.EXPECT_SYSTEM_NSOLVABLES)

class PackageWrapping(base.TestCase):
    class MyPackage(hawkey.Package):
        def __init__(self, initobject, myval):
            super(PackageWrapping.MyPackage, self).__init__(initobject)
            self.initobject = initobject
            self.myval = myval

    def test_wrapping(self):
        sack = hawkey.test.TestSack(repo_dir=self.repo_dir,
                                    PackageClass=self.MyPackage,
                                    package_userdata=42)
        sack.load_rpm_repo()
        pkg = sack.create_package(2)
        # it is the correct type:
        self.assertIsInstance(pkg, self.MyPackage)
        self.assertIsInstance(pkg, hawkey.Package)
        # it received userdata:
        self.assertEqual(pkg.myval, 42)
        # the common attributes are working:
        self.assertEqual(pkg.name, "fool")
