#include "../html.h"

std::string Html::credits(){
    return header("Credits") + "<h2>Credits</h2><div class=\"credits\">"
        "<h4>Staff</h4>"
        "<ul>"
            "<li><a href=\"/user/1\">codl</a></li>"
            "<li><a href=\"/user/2\">fmang</a></li>"
        "</ul>"
        "<h4>Support</h4>"
        "<ul>"
            "<li><a href=\"/user/3\">AspectOfTheStorm</a> - News writer</li>"
            "<li><a href=\"/user/6\">Makkon</a> - Admin of <a href=\"http://mylittleremix.com\">My Little Remix</a></li>"
        "</ul>"
        "<h4>Design</h4>"
        "<ul>"
            "<li><a href=\"http://www.slt.fr\">SLT</a> - Logo</li>"
            "<li><a href=\"http://p.yusukekamiyamane.com/\">Yusuke Kamiyamane</a> - Fugue icon set</li>"
        "</ul></div>"
        + footer();
}
