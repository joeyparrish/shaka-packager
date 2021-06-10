#!/usr/bin/python
#
# Copyright 2018 Google Inc. All rights reserved.
#
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://developers.google.com/open-source/licenses/bsd
"""This script extracts the latest release notes from the changelog."""

from __future__ import print_function

import re
import sys

def main():
  with open('CHANGELOG.md', 'r') as f:
    contents = f.read()

  # This excludes the header line with the release name and date, to match the
  # style of releases done before the automation was introduced.
  entry = re.split(r'^## .*', contents, flags=re.M)[1]

  print(entry)
  return 0

if __name__ == '__main__':
  sys.exit(main())
