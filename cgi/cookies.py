#!/usr/bin/python3
# Import modules for CGI handling 
import cgi
import cgitb 
cgitb.enable()
# Create instance of FieldStorage 
form = cgi.FieldStorage() 
# print (form.keys())
# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')
print("Set-Cookie:first_name=%s;" % (first_name))
print("Set-Cookie:last_name=%s;" % (last_name))
print ("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Cookies in CGI</title>")
print("</head>")
print("<body>")
print("Setting cookies")
print("<br/>")
print("</body>")
print("</html>")
