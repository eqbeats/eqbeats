from flask import Flask
import os
path = os.getenv("TAKEOUT_PATH", "/takeout")

app = Flask("takeout", static_url_path=path+"/static")

@app.route(path + "/")
def index():
    return "Hello, World"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=(os.getenv("TAKEOUT_DEBUG")!=None))
