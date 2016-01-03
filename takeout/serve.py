from flask import Flask, render_template, request, g, redirect, jsonify, make_response
import eqbeats
import os
import takeout
path = os.getenv("TAKEOUT_PATH", "/takeout")

app = Flask("takeout", static_url_path=path+"/static")

@app.before_request
def check_session():
    sid = request.cookies.get('sid')
    g.user = eqbeats.User(sid)
    if not g.user.valid() and not request.endpoint in ('login', 'static'):
        return redirect(path + "/login")

@app.route(path + "/")
def download():
    job = takeout.get_q().fetch_job("userarchive:%s" % (g.user.id,))
    if not job or job.is_failed:
        job = takeout.archive_user(g.user)
    return render_template('index.html', user=g.user, job=job, path=path)

@app.route(path + "/clear")
def clear():
    job = takeout.get_q().fetch_job("userarchive:%s" % (g.user.id,))
    if job:
        job.cancel()
    return redirect(path)

@app.route(path + "/jobstatus")
def jobstatus():
    job = takeout.get_q().fetch_job("userarchive:%s" % (g.user.id,))
    if job:
        return jsonify(status=job.status, result=job.result, meta=job.meta)
    else:
        return jsonify()

@app.route(path + "/login", methods=('GET', 'POST'))
def login():
    if request.method == "POST":
        g.user = eqbeats.User(email = request.form['email'], password = request.form['pw'])
        if not g.user.valid():
            return render_template('login.html', user=g.user, error="Could not find a user with these credentials. Please try again or contact support.")
        else:
            resp = make_response(redirect(path))
            resp.set_cookie('sid', g.user.sid)
            return resp
    else:
        return render_template('login.html', user=g.user)

@app.route(path + "/logout")
def logout():
    if g.user:
        g.user.logout()
    return redirect(path)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8051, debug=(os.getenv("TAKEOUT_DEBUG")!=None))
