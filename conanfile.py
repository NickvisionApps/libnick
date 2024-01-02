from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.build import check_min_cppstd

class libauraRecipe(ConanFile):
    name = "libaura"
    version = "2024.1.0"
    package_type = "library"

    # Optional metadata
    license = "GPLv3"
    author = "Nicholas Logozzo nlogozzo225@gmai.com"
    url = "https://github.com/NickvisionApps/libaura"
    description = "A cross-platform base for native Nickvision applications."
    topics = ("framework", "desktop", "gtk", "winui")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def validate(self):
        check_min_cppstd(self, "20")

    def requirements(self):
        self.requires("boost/1.83.0")
        self.test_requires("gtest/1.14.00")
        self.requires("jsoncpp/1.9.5")
        self.requires("libcurl/8.4.0")
        self.requires("libgettext/0.22")
        self.requires("maddy/1.3.0")
        self.requires("sqlcipher/4.5.1")
        if self.settings.os == "Linux":
            self.requires("glib/2.78.1")
            self.requires("libsecret/0.20.5")
            self.requires("libuuid/1.0.3")

    def source(self):
        git = Git(self)
        git.clone(url="https://github.com/NickvisionApps/libaura.git", target=".")
        git.checkout("2024.1.0-pre")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")
        else:
            self.settings.os.libc = "glibc"
            self.settings.os.libc.version = "2.35"

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if not self.conf.get("tools.build:skip_test", default=False):
            test_folder = os.path
            if self.settings.os == "Windows":
                test_folder = os.path.join("", str(self.settings.build_type))
            self.run(os.path.join(test_folder, "libaura_test"))

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["libaura"]