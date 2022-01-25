#!/usr/bin/python3

# Import modules for CGI handling 
import cgi
import cgitb 
import os
import http.cookies

cgitb.enable()
form = cgi.FieldStorage() 
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')


cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))

if first_name is None:
	first_name = cookie.get("first_name")
	first_name = first_name.value
if last_name is None:
	last_name = cookie.get("last_name")
	last_name = last_name.value

print("Set-Cookie:first_name=%s;" % (first_name))
print("Set-Cookie:last_name=%s;" % (last_name))
print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello <b>%20s%20s</b> </h2>" % (first_name, last_name)) 
print ("</body>")
print ("</html>")