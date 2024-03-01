# setup.py for gala
# (c) 2023, 2024 Fahad Hasan
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 3, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

# see README.python for additional notes.

from setuptools import setup, find_packages, Command
from distutils.command.install_headers import install_headers as install_headers_orig
import os
import subprocess

# this is meant to set up the builddir when using the git repo
# BUG: redundant. Use a list, and a helper function
if not os.path.isfile("MakeList"):
    os.symlink("../MakeList", "MakeList")

if not os.path.isfile("README"):
    os.symlink("../README", "README")

if not os.path.isfile("README"):
    os.symlink("../VERSION", "VERSION")

class InstallHeaders(install_headers_orig):

    def run(self):
        headers = self.distribution.headers or []
        for header in headers:
            dst = os.path.join(self.install_dir, os.path.dirname(header))
            self.mkpath(dst)
            (out, _) = self.copy_file(header, dst)
            self.outfiles.append(out)

class RunTests(Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        errno = subprocess.check_call(['make', '-C', 'tests'])
        raise SystemExit(errno)

class CreateSymlinks(Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        target_example_dir = os.path.join(root_dir, 'examples')
        target_test_dir = os.path.join(root_dir, 'tests')

        link_example_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'examples')
        link_test_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'tests')

        if not os.path.exists(link_example_dir):
            os.symlink(target_example_dir, link_example_dir)
        if not os.path.exists(link_test_dir):
            os.symlink(target_test_dir, link_test_dir)

class parse_makefile:
	def __init__(self, fn):
		self._file = open(fn, "r")	

	def get(self, what, _):
		# BUG: doesn't work right, hardcode below.
		# try:
		# 	  contents = file.read()
		# 	  headers_match = re.search(r'HEADERS = \\(?:\n\t[^\n]+ \\)*\n\t[^\n]+', contents)
		# 	  if headers_match:
		# 			# Extract header lines, strip whitespace and backslashes
		# 			headers_lines = headers_match.group().strip().split('\n')[1:] # Skip 'HEADERS = \' line
		# 			headers = [line.strip('\\').strip() for line in headers_lines]
		# except FileNotFoundError:
		# 	 print("Makefile not found.")
		# except Exception as e:
		# 	 print(f"An error occurred: {str(e)}")
		return "boost_assign.h boost_copy.h boost_detail.h boost.h cbset.h degs.h digraph.h graph.h immutable.h sethack.h sfinae.h td.h trace.h platform.h Makefile"

try:
    makelist = parse_makefile("MakeList")
    headers = makelist.get("HEADERS", "").split()
except Exception as e:
    print(f"Error while parsing MakeList: {str(e)}")
    headers = []

# TODO: possibly not needed when using sdist?
for header in headers:
    source_path = os.path.join("..", header)  # Original headers are in the parent directory
    symlink_path = header  # Symlink to be created in the same directory as setup.py
    
    if not os.path.isfile(symlink_path) and os.path.isfile(source_path):
        os.symlink(source_path, symlink_path)

# BUG: somehow the tarball lacks the headers without assigning "data_files".
#  but with data_files set, they end up in the wheel twice...
#Test Commands:
# 1. python setup.py create_symlinks
# 2. python setup.py test
	 # BUG: get version from VERSION
setup(
    name='gala',
    version='1+dev',
    headers=headers,
    data_files=headers,
    classifiers=[
        'Intended Audience :: Developers',
        'Programming Language :: C++',
        'Operating System :: OS Independent',
    ],
    cmdclass={
        'install_headers': InstallHeaders,
        'test': RunTests,
		  'create_symlinks': CreateSymlinks # BUG: don't expose this.
    }
)
