#!/usr/bin/python3

import fileinput
import json
import sys
import socket
from datetime import date

seen = [];
tids = [];

input = fileinput.input()

def go():
    global seen
    try:
        for line in input:
            if(fileinput.isfirstline()): sys.stderr.write(fileinput.filename() + "\n")
            if(line[0] != '{'): continue
            if(line[-2] == ','): line = line[:-2]
            line = line.replace("'", '"')
            try:
                data = json.loads(line)
            except ValueError:
                continue
            day = date.fromtimestamp(data["timestamp"]).isoformat()

            if "uid" in data:
                uid = data["uid"]
            else:
                uid = int(fileinput.filename().split("/")[-1].split(".")[0])

            print("INCR stat:user:%d:%s" % (uid, data['type']))

            if "tid" in data:
                print("INCR stat:track:%d:%s" % (data['tid'], data['type']))
                print("HINCRBY stat:track:%d:%s:daily %s 1" % (data['tid'], data['type'], day))

                uniq_hash = day+data["type"]+data["addr"]

                if uniq_hash not in seen:
                    seen.append(uniq_hash)
                    print("INCR stat:track:%d:%s:unique" % (data['tid'], data['type']))
                    print("HINCRBY stat:track:%d:%s:daily:unique %s 1" % (data['tid'], data['type'], day))
                    try:
                        print("ZINCRBY stat:track:%d:referrers 1 %s" % (data['tid'], data['referrer']))
                    except KeyError:
                        pass

                seen = seen[-1000:]
                if data['tid'] not in tids:
                    tids.append(data['tid'])
    except UnicodeDecodeError: # broken referrer?
        go(); # this is so gross

go()

hitsd = socket.socket(socket.AF_UNIX)
hitsd.connect("/srv/eqbeats/hitsd.sock")
for tid in tids:
    hitsd.send(("get %d\n" % tid).encode())
    tally = int(hitsd.recv(4096));
    print("SET stat:track:%d:trackView %d" % (tid, tally))

hitsd.close()
