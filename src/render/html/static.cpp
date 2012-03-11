#include "static.h"
#include "page.h"
#include "../render.h"

using namespace Render;

void Html::quickStart(){
    header("Thanks");
    o << "<div class=\"message\">Now that you've registered you can upload your tracks and follow artists: just click your name at the top right corner of the page.</div>"
         "<div class=\"message\">Don't submit any tracks but your own, though.</div>";
    footer();
}

void Html::faq(){
    header("FAQ");
    o << "<h2>Frequently Asked Questions</h2><div class=\"faq\">"

        "<div><h4>What is this?</h4>"
        "<p>A place for artists to publish their MLP-related music, and get in touch with their audience.</p></div>"

        "<div><h4>Why?</h4>"
        "<p>Because there is no other central repository for MLP music.</p></div>"

        "<div><h4>Why not [INSERT SITE HERE]?</h4>"
        "<p>Because we try to make the publishing and listening process as smooth as can be. No ads, no limits, no downsides.</p></div>"

        "<div><h4>Who are you anyway?</h4>"
        "<p>We're two developers: <a href=\"/user/1\">codl</a> and <a href=\"/user/2\">fmang</a>. For more information check the <a href=\"/credits\">credits</a>.</p></div>"

        "<div><h4>I've got a question/problem/idea!</h4>"
        "<p>What are you waiting for? Drop us a line at contact@eqbeats.org.</p></div>"

        "</div>";
    footer();
}

void Html::credits(){
    header("Credits");
    o << "<h2>Credits</h2><div class=\"credits\">"
        "<h4>Staff</h4>"
        "<ul>"
            "<li><a href=\"/user/1\">codl</a></li>"
            "<li><a href=\"/user/2\">fmang</a></li>"
        "</ul>"
        "<h4>Support</h4>"
        "<ul>"
            "<li><a href=\"/user/3\">AspectOfTheStorm</a></li>"
            "<li><a href=\"/user/6\">Makkon</a></li>"
            "<li><a href=\"/user/68\">Karai</a></li>"
        "</ul>"
        "<h4>Design</h4>"
        "<ul>"
            "<li><a href=\"http://www.slt.fr\">SLT</a> - Logo</li>"
            "<li><a href=\"http://blackm3sh.deviantart.com/art/Rarity-Filly-203377463\">BlackM3sh</a> - Filly Rarity</li>"
            "<li><a href=\"http://p.yusukekamiyamane.com/\">Yusuke Kamiyamane</a> - Fugue icon set</li>"
        "</ul></div>";
    footer();
}
