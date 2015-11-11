from flask import Flask, render_template, request, g, redirect
import eqbeats
import os
path = os.getenv("TAKEOUT_PATH", "/takeout")

app = Flask("takeout", static_url_path=path+"/static")

@app.before_request
def check_session():
    sid = request.cookies.get('sid')
    g.user = eqbeats.User(sid)
    if not g.user.valid():
        return redirect("/login?redirect=/takeout")

@app.route(path + "/")
def index():
    return render_template('index.html', user=g.user)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=(os.getenv("TAKEOUT_DEBUG")!=None))
