
#include <iostream>

using namespace std;

int main(int argc, char **argv, char** env)
{
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>CGI Envrionment Variables</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";

    while (*env)
        cout << *env++ << "<br/>";

    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}