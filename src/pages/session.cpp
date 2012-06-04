PATH("/login"){

    // Default page
    HTML("Login");
    tpl = "login.tpl";

    // Redirect if logged
    std::string sid;
    if(Session::login(dict, &sid)){
        if(!sid.empty()) o << "Set-Cookie: sid=" << sid << ";Max-Age=2592000\n"; // 30 days
        redir = cgi("redirect").empty() ? Session::user().url() : cgi("redirect");
    }

}

PATH("/logout"){
    Session::logout();
    o << "Set-Cookie: sid=\n";
    redir = cgi("redirect").empty() ? "/" : cgi("redirect");
}
