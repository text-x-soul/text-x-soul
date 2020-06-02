from distutils.core import setup

from os import path
this_directory = path.abspath(path.dirname(__file__))
with open(path.join(this_directory, 'README.rst'), encoding='utf-8') as f:
    long_description = f.read()

setup(
  name = 'PySoul',
  long_description=long_description,
  long_description_content_type='text/x-rst',
  packages = ['PySoul'],
  version = '0.5.5',
  license='CC0 1.0 Universal',
  description = 'Python Implementation for handling Soul Configuration files',
  author = 'Yet-Zio',
  author_email = 'maheshdeluxe53@gmail.com',
  url = 'https://text-x-soul.tk',
  download_url = 'https://github.com/text-x-soul/text-x-soul/raw/master/Python/0.5.2/releases/0.5.5/PySoul-0.5.5.tar.gz',
  keywords = ['PUBLIC', 'SERIALIZATION', 'CONFIGURATION'],
  classifiers=[
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: Developers',
    'Topic :: Software Development :: Libraries',
    'License :: CC0 1.0 Universal (CC0 1.0) Public Domain Dedication',
    'Programming Language :: Python :: 3',
    'Programming Language :: Python :: 3.4',
    'Programming Language :: Python :: 3.5',
    'Programming Language :: Python :: 3.6',
    'Programming Language :: Python :: 3.7',
    'Programming Language :: Python :: 3.8',
  ],
)
