#!/usr/bin/python3

# Import modules for CGI handling 
import cgi
import cgitb 
cgitb.enable()

# Create instance of FieldStorage 
form = cgi.FieldStorage() 
print (form.keys())
# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello <b>%20s%20s</b> </h2>" % (first_name, last_name)) 
print ("</body>")
print ("</html>")