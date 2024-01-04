import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import cmake_layout
from conan.tools.build import check_min_cppstd

class libauraRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def validate(self):
        check_min_cppstd(self, "20")
        if self.settings.os != "Windows" and self.settings.os != "Linux":
            raise ConanInvalidConfiguration("Only Windows and Linux are supported by libaura.")

    def requirements(self):
        self.requires("boost/1.83.0")
        self.requires("gtest/1.14.0")
        self.requires("jsoncpp/1.9.5")
        self.requires("libcurl/8.4.0")
        self.requires("libgettext/0.22")
        self.requires("maddy/1.3.0")
        self.requires("sqlcipher/4.5.1")
        if self.settings.os == "Linux":
            self.requires("glib/2.78.1")
            self.requires("libsecret/0.20.5")
            self.requires("libuuid/1.0.3")

    def layout(self):
        cmake_layout(self)