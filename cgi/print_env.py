#!/usr/bin/python
import os
# print("<font size=+10>Environment</font><br>")

for param in os.environ.keys():
    print("<b>%20s</b>: %s<br>" % (param, os.environ[param]))