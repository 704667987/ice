#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

import sys, os, signal

try:
    import demoscript
except ImportError:
    for toplevel in [".", "..", "../..", "../../..", "../../../.."]:
        toplevel = os.path.normpath(toplevel)
        if os.path.exists(os.path.join(toplevel, "demoscript")):
            break
    else:
        raise "can't find toplevel directory!"
    sys.path.append(os.path.join(toplevel))
    import demoscript

import demoscript.Util
demoscript.Util.defaultLanguage = "C++"

if demoscript.Util.isDarwin():
    print "This demo is not supported under MacOS."
    sys.exit(0)

print "cleaning databases...",
sys.stdout.flush()
demoscript.Util.cleanDbDir("db")
print "ok"

print "testing IceUtl::Cache evictor"
server = demoscript.Util.spawn('./server --Ice.PrintAdapterReady')
server.expect(".* ready", timeout=120)

client = demoscript.Util.spawn('./client')
client.waitTestSuccess(timeout=8 * 60)
print client.before

server.kill(signal.SIGINT)
server.waitTestSuccess(timeout=60)

print "testing simple evictor"
server = demoscript.Util.spawn('./server simple --Ice.PrintAdapterReady')
server.expect(".* ready")

client = demoscript.Util.spawn('./client')
client.waitTestSuccess(timeout=8*60)
print client.before

server.kill(signal.SIGINT)
server.waitTestSuccess(timeout=60)
